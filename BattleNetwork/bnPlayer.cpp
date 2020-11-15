#include "bnPlayer.h"
#include "bnNaviWhiteoutState.h"
#include "bnField.h"
#include "bnBusterCardAction.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"
#include "bnAura.h"

#include "bnBubbleTrap.h"
#include "bnBubbleState.h"

#define RESOURCE_PATH "resources/navis/megaman/megaman.animation"

Player::Player() :
  state(PLAYER_IDLE),
  chargeEffect(this),
  AI<Player>(this),
  Character(Rank::_1)
{
  ChangeState<PlayerIdleState>();
  
  // The charge component is also a scene node
  // Make sure the charge is in front of this node
  // Otherwise children scene nodes are drawn behind 
  // their parents
  chargeEffect.SetLayer(-2);
  AddNode(&chargeEffect);
  chargeEffect.setPosition(0, -20.0f); // translate up -20
  chargeEffect.EnableParentShader(false);

  SetLayer(0);
  team = Team::red;

  setScale(2.0f, 2.0f);

  animationComponent = CreateComponent<AnimationComponent>(this);
  animationComponent->SetPath(RESOURCE_PATH);
  animationComponent->Reload();

  previous = nullptr;
  playerControllerSlide = false;
  activeForm = nullptr;

  auto recoil = [this]() {
    // When movement is interrupted because of a hit, we need to flush the movement state data
    FinishMove();
    ChangeState<PlayerHitState>();
  };

  this->RegisterStatusCallback(Hit::recoil, Callback<void()>{ recoil });

  auto bubbleState = [this]() {
    ChangeState<BubbleState<Player>>();
  };

  this->RegisterStatusCallback(Hit::bubble, Callback<void()>{ bubbleState});
}

Player::~Player() {
}

void Player::OnUpdate(float _elapsed) {
  if (GetTile() != nullptr) {
    setPosition(tileOffset.x + GetTile()->getPosition().x, tileOffset.y + GetTile()->getPosition().y);
  }

  AI<Player>::Update(_elapsed);

  if (activeForm) {
    activeForm->OnUpdate(_elapsed, *this);
  }

  //Node updates
  chargeEffect.Update(_elapsed);
}

void Player::Attack() {
  // Queue an action for the controller to fire at the right frame
  if (tile) {
    chargeEffect.IsFullyCharged() ? QueueAction(ExecuteChargedBuster()) : QueueAction(ExecuteBuster());
  }
}

void Player::UseSpecial()
{
  if (tile) {
    auto action = ExecuteSpecial();
    
    if (action) {
      action->SetLockoutGroup(ActionLockoutGroup::ability);
      QueueAction(action);
    }

  }
}

void Player::OnDelete() {
  chargeEffect.Hide();
  auto animationComponent = GetFirstComponent<AnimationComponent>();

  if (animationComponent) {
      animationComponent->CancelCallbacks();
      animationComponent->SetAnimation(PLAYER_HIT);
  }

  ChangeState<NaviWhiteoutState<Player>>();
}

const float Player::GetHeight() const
{
  return 101.0f;
}

int Player::GetMoveCount() const
{
  return Entity::GetMoveCount();
}

void Player::SetCharging(bool state)
{
  chargeEffect.SetCharging(state);
}

void Player::SetAtkLevel(unsigned atk)
{
  atkLevel = std::min(5u, atk);
}

const unsigned Player::GetAtkLevel()
{
  return atkLevel;
}

void Player::SetAnimation(string _state, std::function<void()> onFinish) {
  state = _state;

  if (state == PLAYER_IDLE) {
    animationComponent->SetAnimation(_state, Animator::Mode::Loop);
  }
  else {
    animationComponent->SetAnimation(_state, 0, onFinish);
  }
}

void Player::EnablePlayerControllerSlideMovementBehavior(bool enable)
{
  playerControllerSlide = enable;
}

const bool Player::PlayerControllerSlideEnabled() const
{
  return playerControllerSlide;
}

CardAction* Player::ExecuteBuster()
{
    return OnExecuteBusterAction();
}

CardAction* Player::ExecuteChargedBuster()
{
    return OnExecuteChargedBusterAction();
}

CardAction* Player::ExecuteSpecial()
{
  auto actions = this->GetComponentsDerivedFrom<CardAction>();
  bool canUse = true;

  // We could be using an ability, make sure we do not use another ability
  for (auto&& action : actions) {
    canUse = canUse && action->GetLockoutGroup() != ActionLockoutGroup::ability;
  }

  if (!canUse) {
    return nullptr;
  }
  
  return OnExecuteSpecialAction();
}

void Player::ActivateFormAt(int index)
{
  index = index - 1; // base 1 to base 0

  if (activeForm) {
    activeForm->OnDeactivate(*this);
    delete activeForm;
    activeForm = nullptr;
  }

  if (index >= 0 || index < forms.size()) {
    auto meta = forms[index];
    activeForm = meta->BuildForm();

    if (activeForm) {
      activeForm->OnActivate(*this);
    }
  }
}

void Player::DeactivateForm()
{
  if (activeForm) {
    activeForm->OnDeactivate(*this);
  }
}

const bool Player::IsInForm() const
{
  return (this->activeForm != nullptr);
}

const std::vector<PlayerFormMeta*> Player::GetForms()
{
  auto res = std::vector<PlayerFormMeta*>();

  for (int i = 0; i < forms.size(); i++) {
    res.push_back(forms[i]);
  }

  return res;
}

bool Player::RegisterForm(PlayerFormMeta * info)
{
  if (forms.size() >= Player::MAX_FORM_SIZE || !info) return false;
  forms.push_back(info);
  return true;
}

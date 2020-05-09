#include "bnBusterCardAction.h"
#include "bnCardAction.h"
#include "bnSpriteProxyNode.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnBuster.h"

#define NODE_PATH "resources/spells/buster_shoot.png"
#define NODE_ANIM "resources/spells/buster_shoot.animation"

BusterCardAction::BusterCardAction(Character * owner, bool charged, int damage) : CardAction(owner, "PLAYER_SHOOTING", &attachment2, "Buster"), 
attachmentAnim(owner->GetFirstComponent<AnimationComponent>()->GetFilePath()) {
  BusterCardAction::damage = damage;
  BusterCardAction::charged = charged;

  attachment2 = new SpriteProxyNode();
  attachment2->setTexture(owner->getTexture());
  attachment2->SetLayer(-1);

  attachmentAnim2 = Animation(owner->GetFirstComponent<AnimationComponent>()->GetFilePath());
  attachmentAnim2.Reload();
  attachmentAnim2.SetAnimation("BUSTER");

  attachment = new SpriteProxyNode();
  attachment->setTexture(TextureResourceManager::GetInstance().LoadTextureFromFile(NODE_PATH));
  attachment->SetLayer(-1);

  attachmentAnim = Animation(NODE_ANIM);
  attachmentAnim.Reload();
  attachmentAnim.SetAnimation("DEFAULT");

  isBusterAlive = false;
}

void BusterCardAction::Execute() {
  auto user = GetUser();

  user->AddNode(attachment2);
  attachment2->AddNode(attachment);
  attachmentAnim.Update(0, attachment->getSprite());

  attachment2->EnableParentShader(true);
  attachmentAnim2.Update(0, attachment2->getSprite());

  // On shoot frame, drop projectile
  auto onFire = [this, user]() -> void {
    Buster* b = new Buster(user->GetField(), user->GetTeam(), charged, damage);
    b->SetDirection(Direction::right);
    auto props = b->GetHitboxProperties();
    b->SetHitboxProperties(props);

    isBusterAlive = true;
    Entity::RemoveCallback& busterRemoved = b->CreateRemoveCallback();
    busterRemoved.Slot([this]() {
      isBusterAlive = false;
    });

    user->GetField()->AddEntity(*b, *user->GetTile());
    AUDIO.Play(AudioType::BUSTER_PEA);
  };

  AddAction(1, onFire);
}

BusterCardAction::~BusterCardAction()
{
  if (attachment) {
    delete attachment;
  }

  if (attachment2) {
    delete attachment2;
  }
}

void BusterCardAction::OnUpdate(float _elapsed)
{
  if (attachment) {
    CardAction::OnUpdate(_elapsed);

    attachmentAnim2.Update(_elapsed, attachment2->getSprite());
    attachmentAnim.Update(_elapsed, attachment->getSprite());

    // update node position in the animation
    auto baseOffset = attachmentAnim2.GetPoint("endpoint");
    auto origin = attachment2->getOrigin();
    baseOffset = baseOffset - origin;

    attachment->setPosition(baseOffset);
  }
  else if (!isBusterAlive) {
    EndAction();
  }
}

void BusterCardAction::EndAction()
{
  if (attachment) {
    GetUser()->RemoveNode(attachment2);
    attachment2->RemoveNode(attachment);

    attachment = nullptr;
    attachment2 = nullptr;
  }

  if (isBusterAlive) return; // Do not end action if buster is still on field

  GetUser()->EndCurrentAction();
}

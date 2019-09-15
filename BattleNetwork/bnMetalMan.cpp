#include "bnMetalMan.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnEngine.h"
#include "bnNaviExplodeState.h"
#include "bnObstacle.h"
#include "bnHitBox.h"

#define RESOURCE_PATH "resources/mobs/metalman/metalman.animation"

MetalMan::MetalMan(Rank _rank)
  :
  AI<MetalMan>(this), Character(_rank) {
  name = "MetalMan";
  this->team = Team::BLUE;

  if (rank == Rank::EX) {
    SetHealth(1300);
  }
  else {
    SetHealth(1000);
  }

  this->ShareTileSpace(true); // mega can walk into him on red tiles
  
  hitHeight = 64;
  state = MOB_IDLE;
  healthUI = new MobHealthUI(this);

  setTexture(*TEXTURES.GetTexture(TextureType::MOB_METALMAN_ATLAS));

  setScale(2.f, 2.f);

  this->SetHealth(health);
  this->SetFloatShoe(true);

  //Components setup and load
  animationComponent = new AnimationComponent(this);
  this->RegisterComponent(animationComponent);
  animationComponent->Setup(RESOURCE_PATH);
  animationComponent->Reload();
  animationComponent->SetAnimation(MOB_IDLE);

  animationComponent->OnUpdate(0);

  movedByStun = false;

  hit = false;
}

MetalMan::~MetalMan() {
}

void MetalMan::OnFrameCallback(int frame, std::function<void()> onEnter, std::function<void()> onLeave, bool doOnce) {
  animationComponent->AddCallback(frame, onEnter, onLeave, doOnce);
}

bool MetalMan::CanMoveTo(Battle::Tile * next)
{
  if (!next->ContainsEntityType<Character>() && !next->ContainsEntityType<Obstacle>()) {
    return true;
  }

  return false;
}

void MetalMan::OnUpdate(float _elapsed) {
  // TODO: use StuntDoubles to circumvent teleportaton
  if (movedByStun) { 
    this->Teleport((rand() % 3) + 4, (rand() % 3) + 1); 
    this->AdoptNextTile(); 
    this->FinishMove();
    movedByStun = false; 
  }

  // todo: add this in Agent::Update() for ALL agent
  if (this->GetTarget() && this->GetTarget()->IsDeleted()) {
    this->SetTarget(nullptr);
  }

  if (!hit) {
    this->SetShader(nullptr);
  }

  setPosition(tile->getPosition().x + this->tileOffset.x, tile->getPosition().y + this->tileOffset.y);

  this->AI<MetalMan>::Update(_elapsed);

  // Explode if health depleted

  HitBox* hitbox = new HitBox(GetField(), GetTeam(), 40);
  auto props = hitbox->GetHitboxProperties();
  props.flags |= Hit::impact | Hit::recoil | Hit::flinch;
  hitbox->SetHitboxProperties(props);

  field->AddEntity(*hitbox, tile->GetX(), tile->GetY());

  hit = false;
}

const bool MetalMan::OnHit(const Hit::Properties props) {
  bool result = true;

  if ((props.flags & Hit::stun) == Hit::stun) {
    if (!Teammate(this->GetTile()->GetTeam())) {
      movedByStun = true;
    }
  }

  hit = result;

  return result;
}

const float MetalMan::GetHitHeight() const {
  return hitHeight;
}

void MetalMan::SetCounterFrame(int frame)
{
  auto onFinish = [&]() { this->ToggleCounter(); };
  auto onNext = [&]() { this->ToggleCounter(false); };
  animationComponent->AddCallback(frame, onFinish, onNext);
}

void MetalMan::SetAnimation(string _state, std::function<void()> onFinish) {
  state = _state;
  animationComponent->SetAnimation(_state, onFinish);
  animationComponent->OnUpdate(0);
}

void MetalMan::OnDelete() {
  this->ChangeState<NaviExplodeState<MetalMan>>(9, 0.75); // freezes animation
}
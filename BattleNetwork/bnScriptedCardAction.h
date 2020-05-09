#pragma once
#include "bnCardAction.h"
#include "bnAnimation.h"
#include <SFML/Graphics.hpp>

class SpriteProxyNode;
class Character;
class ScriptedCardAction : public CardAction {
public:
  ScriptedCardAction(Character * owner, int damage) : CardAction(owner, "PLAYER_IDLE", nullptr, "Buster") {
    // SCRIPTS.callback(card_name).OnCreate(this);
  }

  ~ScriptedCardAction()
  {
  }

  void OnUpdate(float _elapsed)
  {
    CardAction::OnUpdate(_elapsed);

    // SCRIPTS.callback(card_name).OnUpdate(this);
  }

  void EndAction()
  {
    GetUser()->EndCurrentAction();
    // SCRIPTS.callback(card_name).OnEndAction(this);
  }

  void Execute() {

  }
};

#pragma once

#include "bnCardAction.h"
#include "bnAnimation.h"
#include <SFML/Graphics.hpp>

class SpriteProxyNode;
class Character;
class BusterCardAction : public CardAction {
private:
  SpriteProxyNode *attachment, *attachment2;
  Animation attachmentAnim, attachmentAnim2;
  bool charged;
  int damage;
  bool isBusterAlive;
public:
  BusterCardAction(Character* owner, bool charged, int damage);
  ~BusterCardAction();
  void OnUpdate(float _elapsed);
  void OnAnimationEnd();
  void EndAction();
  void Execute();
};

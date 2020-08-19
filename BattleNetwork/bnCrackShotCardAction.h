#pragma once
#include "bnCardAction.h"
#include "bnAnimation.h"
#include <SFML/Graphics.hpp>

class SpriteProxyNode;
class Character;

class CrackShotCardAction : public CardAction {
private:
  sf::Sprite overlay;
  SpriteProxyNode* attachment;
  Animation attachmentAnim;
  int damage;
public:
  CrackShotCardAction(Character* owner, int damage);
  ~CrackShotCardAction();
  void OnUpdate(float _elapsed);
  void OnAnimationEnd();
  void EndAction();
  void Execute();
};

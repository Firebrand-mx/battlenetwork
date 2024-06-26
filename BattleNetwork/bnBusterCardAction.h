#pragma once

#include "bnCardAction.h"
#include "bnAnimation.h"
#include "bnField.h"
#include <SFML/Graphics.hpp>

class SpriteProxyNode;
class Character;
class BusterCardAction : public CardAction {
private:
  Attachment* busterAttachment{ nullptr };
  bool charged{};
  int damage{};
  std::shared_ptr<SpriteProxyNode> buster;
  Field::NotifyID_t notifier{ -1 };
public:
  BusterCardAction(std::weak_ptr<Character> user, bool charged, int damage);

  void Update(double _elapsed) override;
  void OnAnimationEnd();
  void OnActionEnd();
  void OnExecute(std::shared_ptr<Character> user);
};

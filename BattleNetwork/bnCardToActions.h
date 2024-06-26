#pragma once
#include <memory>
#include "bnCard.h"
class Character;
class CardPackagePartitioner;

/***
 * all of this code will be tossed out when scripting cards is complete.
 * This is for demonstration of the engine until we have scripting done
 */

std::shared_ptr<CardAction> CardToAction(
  const Battle::Card& card, 
  std::shared_ptr<Character> character, 
  CardPackagePartitioner* partition,
  std::optional<Battle::Card::Properties> moddedProps
);
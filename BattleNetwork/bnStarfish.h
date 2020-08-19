/*! \brief Starfish is Aqua type and spawns bubbles */

#pragma once
#include "bnCharacter.h"
#include "bnAI.h"
#include "bnTextureType.h"
#include "bnStarfishIdleState.h"
#include "bnAnimationComponent.h"

class Starfish : public Character, public AI<Starfish> {
  friend class StarfishIdleState;
  friend class StarfishAttackState;

public:
  using DefaultState = StarfishIdleState;

  Starfish(Rank _rank = Rank::_1);
  ~Starfish();

  /**
   * @brief Updates health ui, AI, and super classes
   * @param _elapsed in seconds
   */
  void OnUpdate(float _elapsed) override;

  void OnDelete() override;

  /**
   * @brief Set the hit height for projectiles to play effects at the correct position
   * @return Y offset
   */
  const float GetHeight() const override;

private:
  float hitHeight;
  TextureType textureType;
  DefenseRule* virusBody;
  AnimationComponent* animation;
};

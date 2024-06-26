/*! \brief FadeIn intro effect used in the battle intro
 *
 * This state can be used by any Entity in the engine.
 * It uses constraints to ensure the type passed in Any
 * is a subclass of Entity.
 *
 * This state locks the entity until it comes into focus
 */

#pragma once
#include "bnEntity.h"
#include "bnAIState.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include <iostream>

typedef std::function<void()> FinishNotifier;

template<typename Any>
class FadeInState : public AIState<Any>
{
private:
  double factor; /*!< Strength of the pixelate effect. Set to 125 */
  FinishNotifier callback; /*!< Callback when intro effect finished */
public:
  inline static const int PriorityLevel = 2;

  /**
   * \brief sets the finish callback and loads shader
   */
  FadeInState(FinishNotifier onFinish);

  /**
   * @brief deconstructor
   */
  ~FadeInState();

  /**
   * @brief Plays intro swoosh sound
   * @param e entity
   */
  void OnEnter(Any& e);

  /**
   * @brief When the pixelate effect finishes, invokes the callback
   * @param _elapsed in seconds
   * @param e entity
   */
  void OnUpdate(double _elapsed, Any& e);

  /**
   * @brief Revokes the pixelate shader from the entity
   * @param e entity
   */
  void OnLeave(Any& e);
};

#include "bnField.h"
#include "bnLogger.h"

#define MAX_TIME 0.5 // in seconds

template<typename Any>
FadeInState<Any>::FadeInState(FinishNotifier onFinish) : AIState<Any>() {
  callback = onFinish;
  factor = MAX_TIME;
}

template<typename Any>
FadeInState<Any>::~FadeInState() {
}

template<typename Any>
void FadeInState<Any>::OnEnter(Any& e) {
  // play iconic fade in sound
  e.Audio().Play(AudioType::APPEAR);

  sf::Color start = NoopCompositeColor(e.GetColorMode());
  start.a = 0;
  e.setColor(start);
}

template<typename Any>
void FadeInState<Any>::OnUpdate(double _elapsed, Any& e) {
  factor -= _elapsed;

  if (factor <= 0) {
    factor = 0;

    if (callback) { callback(); callback = nullptr; /* do once */ }
  }

  double range = (MAX_TIME - factor) / MAX_TIME;

  sf::Color start = NoopCompositeColor(e.GetColorMode());
  start.a = static_cast<sf::Uint8>(255 * range);
  e.setColor(start);
}

template<typename Any>
void FadeInState<Any>::OnLeave(Any& e) {}

#undef MAX_TIME
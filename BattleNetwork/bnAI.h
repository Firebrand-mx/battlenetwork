#pragma once
#include "bnMeta.h"
#include "bnAIState.h"
#include "bnEntity.h"
#include "bnAgent.h"
#include "bnNoState.h"

template<typename T>
class AI : public Agent {
private:
  AIState<T>* stateMachine;
  T* ref;
  int lock;

protected:
  enum StateLock {
    Locked,
    Unlocked
   };

public:
  void LockState() {
    lock = AI<T>::StateLock::Locked;
  }

  void UnlockState() {
    lock = AI<T>::StateLock::Unlocked;
  }

  AI(T* _ref) : Agent() { stateMachine = nullptr; ref = _ref; lock = AI<T>::StateLock::Unlocked; }
  ~AI() { if (stateMachine) { delete stateMachine; ref = nullptr; this->FreeTarget(); } }

  template<typename U>
  void ChangeState() {
    _DerivedFrom<U, AIState<T>>();

    if (lock == AI<T>::StateLock::Locked) {
      return;
    }

    if (!stateMachine) {
      stateMachine = new NoState<T>();
    }

    stateMachine->template ChangeState<U>();
  }

  /*
    For states that require arguments, pass the arguments
    e.g. 

    this->ChangeState<PlayerThrowBombState>(200.f, 300, true);
  */
  
template<typename U, typename ...Args>
  void ChangeState(Args... args) {
    _DerivedFrom<U, AIState<T>>();

    if (lock == AI<T>::StateLock::Locked) {
      return;
    }

    if (!stateMachine) {
      stateMachine = new NoState<T>();
    }

    stateMachine->template ChangeState<U>(args...);
  }

  void Update(float _elapsed) {
    if (stateMachine != nullptr) {
      AIState<T>* nextState = stateMachine->Update(_elapsed, *ref);

      if (nextState != nullptr) {

        stateMachine->OnLeave(*ref);

        AIState<T>* oldState = stateMachine;
        stateMachine = nextState;
        stateMachine->OnEnter(*ref);
        delete oldState;
        oldState = nullptr;
      }
    }
  }
};


/*! \brief UIComponent is composed of a SceneNode and a Component class
 * 
 * UIComponents are injected into the battle scene and are drawn 
 * on top of everything else.
 * 
 * These components usually draw the health points of the entity
 * they are attached to but can be used to display other data.
 */ 

#pragma once

#include "bnComponent.h"
#include "bnSceneNode.h"

class BattleSceneBase;

class UIComponent : public Component, public SceneNode {
  bool autodraw{ true };

public:


  UIComponent() = delete;
  /**
   * @brief Attaches this component to the owner
   * @param owner
   */
  UIComponent(std::weak_ptr<Entity> owner) : Component(owner, Component::lifetimes::ui) { ; }
  ~UIComponent() { ; }

  UIComponent(UIComponent&& rhs) = delete;
  UIComponent(const UIComponent& rhs) = delete;

  /**
   * @brief Calls parent SceneNode::draw() 
   * @param target
   * @param states
   */
  virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const {
    SceneNode::draw(target, states);
  };

  /**
   * @brief To be implemented: what happens on update
   * @param _elapsed in seconds
   */
  virtual void OnUpdate(double _elapsed) = 0;
  
  /**
   * @brief To be implemented: what happens when the Battlescene requests injection
   */
  virtual void Inject(BattleSceneBase&) = 0;

  void SetDrawOnUIPass(bool enabled);
  const bool DrawOnUIPass() const;
}; 

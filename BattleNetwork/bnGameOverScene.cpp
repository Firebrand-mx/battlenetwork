#include <Swoosh/ActivityController.h>
#include <Segues/WhiteWashFade.h>

#include "bnGameOverScene.h"
#include "overworld/bnOverworldHomepage.h"

using namespace swoosh::types;

GameOverScene::GameOverScene(swoosh::ActivityController& controller) : Scene(controller) {
  fadeInCooldown = 2.0f;
  gameOverTexture = *Textures().LoadFromFile("resources/scenes/game_over/game_over.png");
  //Separating the texture into its own member allows the texture to live long enough to be drawn
  //within the next function.
  gameOver.setTexture(gameOverTexture);
  gameOver.setScale(2.f, 2.f);
  gameOver.setOrigin(gameOver.getLocalBounds().width / 2, gameOver.getLocalBounds().height / 2);

  leave = false;

  setView(sf::Vector2u(640, 480));
}

GameOverScene::~GameOverScene() {

}

void GameOverScene::onStart() {
  Audio().StopStream();
  Audio().Stream("resources/loops/game_over.ogg");
}

void GameOverScene::onResume() {

}

void GameOverScene::onUpdate(double elapsed) {
  if(!leave) {
    fadeInCooldown -= (float) elapsed;

    if (fadeInCooldown < 0) {
      fadeInCooldown = 0;
    }

    if (fadeInCooldown == 0 && (sf::Touch::isDown(0) || Input().Has(InputEvents::pressed_confirm))) {
      leave = true;
    }
  } else {
    fadeInCooldown += (float) elapsed;

    if (fadeInCooldown > 2.0f) {
      fadeInCooldown = 2.0f;

      leave = false;

      using effect = segue<WhiteWashFade, milliseconds<500>>;
      getController().push<effect::to<Overworld::Homepage>>();
    }
  }
}

void GameOverScene::onDraw(sf::RenderTexture& surface) {
  sf::Vector2f logoPos = (sf::Vector2f)((sf::Vector2i)getController().getVirtualWindowSize() / 2);
  gameOver.setPosition(logoPos);
  //gameOver.setColor(sf::Color(255, 255, 255, (sf::Uint32)(255 * (1.0f-(fadeInCooldown / 2.f)))));
  //Commented out because it fades in just fine without it. This causes it to flicker once.
  surface.draw(gameOver);
}

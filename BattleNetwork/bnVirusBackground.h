/*! \brief Virus background uses Background class to animate and scroll
 */
 
#pragma once
#include <SFML/Graphics.hpp>
using sf::Texture;
using sf::Sprite;
using sf::IntRect;
using sf::Drawable;
#include <vector>
using std::vector;

#include "bnBackground.h"

class VirusBackground final : public Background
{
public:
  VirusBackground();
  ~VirusBackground();

  void Update(double _elapsed) override;
  void ScrollUp();
  void ScrollLeft();
  void SetScrollSpeed(const double);
private:
  float x, y;
  double progress;
  double speed;
  int lr{ 0 };
  int ud{ -1 };
};


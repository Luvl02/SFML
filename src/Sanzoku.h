#pragma once
#include "Enemy.h"

class Sanzoku : public Enemy
{
  private:
    sf::Texture standingTexture;
    sf::Texture runTexture;
    sf::Texture attackTexture;
    sf::Texture damageTexture;

    int animationFrame = 0;
    int maxFrames = 5;
    float frameDuration = .15;
    sf::Clock animationClock;

  public:
  
    Sanzoku(float startX, float startY);

    ~Sanzoku() override;

    sf::FloatRect getGlobalBounds() const override;


    void Load() override;
    void Update(float deltaTime, sf::Vector2f playerPostions) override;

};
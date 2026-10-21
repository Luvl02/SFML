#pragma once
#include "Enemy.h"

class Sanzoku : public Enemy
{
  private:
    sf::Texture standingTexture;
    sf::Texture runTexture;
     std::vector<sf::Texture> attackTextures;
    sf::Texture damageTexture;
    sf::Texture deathTexture;

    int animationFrame = 0;
    int maxFrames = 5;
    float frameDuration = .15;
    sf::Clock animationClock;

    bool isAttacking = false;
    int attackCount = -1;
    int attackType;
    int lastAttack = -1;
    float attackCooldown = 0.f;



  public:
  
    Sanzoku(float startX, float startY);

    ~Sanzoku() override;

    sf::FloatRect getGlobalBounds() const override;


    void Load() override;
    void Update(float deltaTime, sf::Vector2f playerPostions) override;

};
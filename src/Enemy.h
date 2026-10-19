#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
  protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i size;

    float enemySpeed;
    float velocityY = 0.f;
    float gravity = .035;
    bool isGrounded = false;
    float groundLevel = 608.f;

  public:
    Enemy(float startX, float startY);
    virtual ~Enemy();

    virtual void Load() = 0;
    virtual void Update(float deltaTime, sf::Vector2f playerPosition) = 0;

    void applyGravity(float deltaTime);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
};
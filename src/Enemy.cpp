#include "Enemy.h"

Enemy::Enemy(float startX, float startY):
sprite( texture )
{
   sprite.setPosition(sf::Vector2f(startX, startY));
  sprite.setOrigin({96.f / 2.f, 96.f / 2.f}); // Centers origin like your player
  sprite.setScale(sf::Vector2f(2.f, 2.f));     // Matches your player scale
}

Enemy::~Enemy()
{
}

void Enemy::applyGravity(float deltaTime)
{
    if (!isGrounded)
    {
        velocityY += gravity * deltaTime;
        sf::Vector2f position = sprite.getPosition();
        position.y += velocityY;

        // Ground collision check
        if (position.y >= groundLevel)
        {
            position.y = groundLevel;
            velocityY = 0.f;
            isGrounded = true;
        }
        sprite.setPosition(position);
    }
}

void Enemy::Draw(sf::RenderWindow &window)
{
  window.draw( sprite );
}

sf::Vector2f Enemy::getPosition() const
{
  return sprite.getPosition();
}

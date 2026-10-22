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

sf::FloatRect Enemy::getGlobalBounds() const
{
  return sprite.getGlobalBounds();
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
  //window.draw(debugHitbox);
  //window.draw( rectangle );
}

sf::Vector2f Enemy::getPosition() const
{
  return sprite.getPosition();
}

bool Enemy::tookDamage()
{
  bool tookDmg;
  if(this-> healthIndex > this-> health)
  {
    tookDmg = true;
  }
  else
  {
    tookDmg = false;
  }
  this-> healthIndex = this-> health;
  return tookDmg;
}

void Enemy::decreaseHealth()
{
  if(health > 0)
  {
    health -= 50;
  }
}



bool Enemy::isDead()
{
  if(health <= 0 && isDeathAnimationFinished)
  { 
    return true;
  }
  else
  {
    return false;
  }
}

bool Enemy::hasHitPlayer() const
{
  return hasDealtDamage;
}

void Enemy::setHitPlayer(bool hit)
{
  hasDealtDamage = hit;
}

sf::FloatRect Enemy::getAttackBounds() const
{
  if (!isAttacking)
  {
    return sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
  }
  return enemyAttackBox;
}

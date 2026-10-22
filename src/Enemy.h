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
    int healthIndex;
    float hurtTimer = 0.f;

    sf::FloatRect enemyAttackBox;
    bool enemyFacingRight = true;
    sf::RectangleShape rectangle;
    bool isAttacking = false;
    bool hasDealtDamage = false;

  public:
  sf::RectangleShape debugHitbox;
    int health;
    bool isDeathAnimationFinished = false; 

    public:
    Enemy(float startX, float startY);
    virtual ~Enemy();

    virtual void Load() = 0;
    virtual void Update(float deltaTime, sf::Vector2f playerPosition) = 0;

    virtual sf::FloatRect getGlobalBounds() const;

    void applyGravity(float deltaTime);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    bool tookDamage();
    void decreaseHealth();
    bool isDead();

    bool hasHitPlayer() const;
    void setHitPlayer(bool hit);
    bool attackAnimationFinished = false;

    sf::FloatRect getAttackBounds() const;

};
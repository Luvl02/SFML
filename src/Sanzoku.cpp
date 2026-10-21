#include "Sanzoku.h"
#include <iostream>
#include <cmath>

Sanzoku::Sanzoku(float startX, float startY): Enemy(startX, startY)
{
  enemySpeed = .3;
  health = 200;
  healthIndex = health;
}

Sanzoku::~Sanzoku()
{
}

sf::FloatRect Sanzoku::getGlobalBounds() const
{
    sf::Vector2f pos = sprite.getPosition();
    
    // Custom tight box ignoring transparent padding
    return sf::FloatRect(
        {pos.x - 30.f, pos.y}, 
        {60.f, 60.f}                 
    );
}


void Sanzoku::Load()
{
    if (standingTexture.loadFromFile("assets/Enemies/sanzoku/IDLE.png") && attackTexture.loadFromFile("assets/Enemies/sanzoku/RUN.png") && damageTexture.loadFromFile("assets/Enemies/sanzoku/HURT.png"))
    {
        std::cout << "Sanzoku Texture Loaded Successfully" << std::endl;
    }
    else
    {
        std::cout << "Failed to Load Sanzoku Texture!" << std::endl;
    }

    sprite.setTexture( standingTexture );
    sprite.setTextureRect(sf::IntRect({0, 0}, {96, 96})); // Assuming 96x96 frames like your player
    sprite.setOrigin({96.f / 2.f, 25.f});
    sprite.setScale(sf::Vector2f(2.f, 2.f));

    debugHitbox.setFillColor(sf::Color::Transparent);
    debugHitbox.setOutlineColor(sf::Color::Cyan);
    debugHitbox.setOutlineThickness(2.f);
}


void Sanzoku::Update(float deltaTime, sf::Vector2f playerPosition)
{
  // Print these when you get close to see what coordinates are being passed
  sf::FloatRect bounds = getGlobalBounds();
    debugHitbox.setPosition({bounds.position.x, bounds.position.y});
    debugHitbox.setSize({bounds.size.x, bounds.size.y});


  sf::Vector2f position = sprite.getPosition();

  float distanceX = playerPosition.x - position.x;
  float absoluteDistance = std::abs( distanceX );
  position.x = std::max(0.f, std::min(position.x, 1280.f));

  // 2. AI Chase Range (e.g., if player is within 400 pixels)


     if(tookDamage() == true)
    {
        sprite.setTexture( damageTexture );
        maxFrames = 4;
        animationFrame = 0;

        sprite.setTextureRect(sf::IntRect({animationFrame * 96, 0}, {96, 96}));

        hurtTimer = 0.7f;
        animationClock.restart();
    }
    if (hurtTimer > 0.f)
    {
      hurtTimer -= deltaTime / 1000.0f; // Decrement timer based on your deltaTime scale
    }
    else
    {
    if (absoluteDistance < 400.f && absoluteDistance > 85.f)
    {
      sprite.setTexture( attackTexture );
      maxFrames = 8;
        if (distanceX > 0)
        {
            // Player is to the right -> Move Right
            position.x += enemySpeed * deltaTime;
            
            // Ensure sprite faces right (scale X is positive 2.f)
            sprite.setScale(sf::Vector2f(2.f, 2.f));
        }
        else
        {
            // Player is to the left -> Move Left
            position.x -= enemySpeed * deltaTime;
            
            // Flip sprite to face left by making scale X negative (-2.f)
            sprite.setScale(sf::Vector2f(-2.f, 2.f));
        }
    }
    else
    {
        sprite.setTexture( standingTexture );
        maxFrames = 5;
    }
    }


    if (animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
        animationFrame++;
        if (animationFrame >= maxFrames)
        {
            animationFrame = 0;
        }

        sprite.setTextureRect(sf::IntRect(
            {animationFrame * 96, 0}, {96, 96}));

        animationClock.restart();
    }
    applyGravity( deltaTime );

    // 3. Update sprite position
    sprite.setPosition(position);
}

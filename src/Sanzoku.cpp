#include "Sanzoku.h"
#include <iostream>
#include <cmath>
#include "Player.h"

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
    if (standingTexture.loadFromFile("assets/Enemies/sanzoku/IDLE.png") && runTexture.loadFromFile("assets/Enemies/sanzoku/RUN.png") && damageTexture.loadFromFile("assets/Enemies/sanzoku/HURT.png") && deathTexture.loadFromFile("assets/Enemies/sanzoku/DEATH.png"))
    {
        std::cout << "Sanzoku Texture Loaded Successfully" << std::endl;
    }
    else
    {
        std::cout << "Failed to Load Sanzoku Texture!" << std::endl;
    }

      attackTextures.resize(3); //4 attack variations
  for(int i = 0; i < 3; i++)
  {
      // Loads ATTACK 0.png, ATTACK 1.png, ATTACK 2.png
      if(attackTextures[i].loadFromFile("assets/ENEMIES/sanzoku/ATTACK " + std::to_string(i) + ".png"))
      {
        std::cout << "Attacks Loaded" << std::endl;
      }
      else
      {
        std::cout << "Failed to load-> Attacks" << std::endl;
      }
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

if(enemyFacingRight)
{
    // Spawn hitbox to the right of the player
    enemyAttackBox = sf::FloatRect({position.x + 25.f, position.y}, {50.f, 50.f});
}
else
{
    // Spawn hitbox to the left of the player
    enemyAttackBox = sf::FloatRect({position.x - 100.f, position.y}, {50.f, 50.f});
}

  // 2. AI Chase Range (e.g., if player is within 400 pixels)
    if (sprite.getScale().x > 0.f)
    {
        enemyFacingRight = true;
    }
    else
    {
        enemyFacingRight= false;
    }

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
      hurtTimer -= deltaTime / 1000.0f; // Decrement timer based on deltaTime scale
    }
    else if(health == 0)
        {
            maxFrames = 9;
            sprite.setTexture( deathTexture );

             if (animationFrame >= maxFrames - 1)
            {
                animationFrame = maxFrames-1; 
                isDeathAnimationFinished = true; 
            }
        }
    else
    {
    if (absoluteDistance < 400.f && absoluteDistance > 85.f && health != 0)
    {
        isAttacking = false;
        frameDuration = .15;
      sprite.setTexture( runTexture );
      maxFrames = 8;
        if (distanceX > 0)
        {
            // Player is to the right -> Move Right
            position.x += enemySpeed * deltaTime;
            
            // Ensure sprite faces right (scale X is positive 2.f)
            sprite.setScale(sf::Vector2f(2.f, 2.f));
        }
        else if(distanceX < 0)
        {
            // Player is to the left -> Move Left
            position.x -= enemySpeed * deltaTime;
            
            // Flip sprite to face left by making scale X negative (-2.f)
            sprite.setScale(sf::Vector2f(-2.f, 2.f));
        }
    }
    else if (absoluteDistance <= 85.f)
    {
        if (!isAttacking)
        {
            isAttacking = true;
            frameDuration = .07;
            hasDealtDamage = false;
            attackAnimationFinished = false;

            rectangle.setPosition({enemyAttackBox.position.x, enemyAttackBox.position.y});
            rectangle.setSize({enemyAttackBox.size.x, enemyAttackBox.size.y});
            
            // Pure random choice between 0, 1, and 2
            attackType = rand() % 3; 

            sprite.setTexture(attackTextures[attackType]);
            
            // Set correct max frames based on variation
            if (attackType == 2)
            {
                maxFrames = 7;
            }
            else
            {
                maxFrames = 5;
            }
            

            animationFrame = 0;
            animationClock.restart();
        }
    }
    else
    {
        if(!isAttacking)
        {
            sprite.setTexture( standingTexture );
            maxFrames = 5;
        }

    }
    }


    if (animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
        animationFrame++;
        if (animationFrame >= maxFrames)
        {
            animationFrame = 0;

            if (isAttacking)
            {
                //frameDuration = .1;
                attackAnimationFinished = true;
                if (absoluteDistance <= 85.f && health > 0)
                {
                    // Randomly roll the next combo attack
                    attackType = rand() % 3;
                    sprite.setTexture(attackTextures[attackType]);
                    hasDealtDamage = false;

                    if (attackType == 2)
                    {
                        maxFrames = 7;
                    }
                    else
                    {
                        maxFrames = 5;
                    }
                }
                else
                {
                    isAttacking = false; // Player moved away, exit attack state
                } 
            }   
            
        }

        

        if(isAttacking)
        {
            int frameWidth =  98;
            int frameHeight = 64;
            
            sprite.setTextureRect(sf::IntRect(
                {animationFrame * frameWidth, 0}, {frameWidth, frameHeight}));
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(
                {animationFrame * 96, 0}, {96, 96}));
        }

        


        animationClock.restart();
    }
    applyGravity( deltaTime );

    // 3. Update sprite position
    sprite.setPosition(position);
}

void Sanzoku::handleDamage(Player& player)
{
     if (!player.isAttacking)
    {
        hasBeenHitThisSwing = false;
        return;
    }
    
     sf::FloatRect playerAttack = player.getAttackBounds();
    if (playerAttack.size.x > 0 && playerAttack.findIntersection(getGlobalBounds()))
    {
      
      if (player.isAttacking)
        {
           sf::FloatRect playerAttack = player.getAttackBounds();
        
            if (playerAttack.size.x > 0 && playerAttack.findIntersection(getGlobalBounds()))
            {
                // If this specific enemy hasn't been hit yet during this attack swing
                if (!hasBeenHitThisSwing)
                {
                    decreaseHealth();
                    hasBeenHitThisSwing = true; // Lock it so this enemy only takes damage once per swing
                    
                    std::cout << health << std::endl;
                    std::cout << "Hit Sanzoku!" << std::endl;
                }
            }
            
        }
    }
}

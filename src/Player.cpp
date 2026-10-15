#include "Player.h"
#include <iostream>

Player::Player():
sprite( standingTexture )
{
  playerSpeed = 2.0;
}

Player::~Player()
{
}

void Player::Initialize()
{
  size = sf::Vector2i( 96, 96);
}

void Player::Load()
{
  if(standingTexture.loadFromFile("assets/PlayerCharacter/standing.png")&&
  runTexture.loadFromFile("assets/PlayerCharacter/RUN.png")&&
  runLeftTexture.loadFromFile("assets/PlayerCharacter/RUN_left.png"))
  {
    std::cout << "Player Image-> Loaded" << std::endl;
  }
  else
  {
    std::cout << "Failed to Load..." << std::endl;
  }
  attackTextures.resize(3); // Assuming you have 3 attack variations
  for(int i = 0; i < 3; i++)
  {
      // Loads ATTACK 0.png, ATTACK 1.png, ATTACK 2.png
      if(attackTextures[i].loadFromFile("assets/PlayerCharacter/ATTACK " + std::to_string(i) + ".png"))
      {
        std::cout << "Attacks Loaded" << std::endl;
      }
      else
      {
        std::cout << "Failed to load" << std::endl;
      }
  }
    sprite.setPosition(sf::Vector2f( 50, 608));

    sprite.setTextureRect(sf::IntRect(
      {Xindex * size.x, Yindex * size.y}, {size.x, size.y}));
      sprite.setOrigin({96.f / 2.f, 96.f / 2.f});

    sprite.setScale(sf::Vector2f(2, 2));
}

void Player::Update()
{

  handleAttack();


  // If we are currently attacking, we usually want to lock movement
  if (isAttacking)
  {
    // Clamp position just in case and return early so movement keys don't fire
    return; 
  }
  handleInput();

    
}

void Player::handleAttack()
{
  // 1. Trigger the attack when K is pressed
  if (!isAttacking && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
  {
    isAttacking = true;
    count++;
    if (count > 2)
    {
      count = 0;
    }

    // Set correct max frames based on your specific attack sheet indices (0-indexed)
    if (count == 0)      maxFrames = 6; // ATTACK 0
    else if (count == 1) maxFrames = 6; // ATTACK 1
    else if (count == 2) maxFrames = 5; // ATTACK 2

    sprite.setTexture(attackTextures[count]);
    attackFrame = 0;
    animationClock.restart();
  }

  // 2. Play the animation frames continuously while attacking
  if (isAttacking)
  {
    if (animationClock.getElapsedTime().asSeconds() >= attackFrameDuration)
    {
      if (attackFrame <= maxFrames)
      {
        sprite.setTextureRect(sf::IntRect(
          {attackFrame * 96, 0}, {96, 96}));

        attackFrame++;
        animationClock.restart();    
      }
      else
      {
        // Attack finished, reset back to standing
        isAttacking = false;
        attackFrame = 0;
        maxFrames = 10;

        sprite.setTexture(standingTexture);
        animationFrame = 0;
        sprite.setTextureRect(sf::IntRect(
          {0, 0}, {96, 96}));
          
        animationClock.restart();
      }
    }
  }
}

void Player::handleInput()
{
  sf::Vector2f position = sprite.getPosition();
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
  {
    
    sprite.setTexture( runTexture );
    
    sprite.setPosition(sf::Vector2f(position + sf::Vector2f( 1 , 0 ) * playerSpeed));

    if(animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
      maxFrames = 16;
      animationFrame++;

      if(animationFrame >= maxFrames)
      {
        animationFrame = 0;
      }

      sprite.setTextureRect(sf::IntRect(
        {animationFrame * 96, Yindex * 96}, {96, 96}));

        animationClock.restart();
    }
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
  {
    sprite.setTexture( runLeftTexture );

    sprite.setPosition(sf::Vector2f(position + sf::Vector2f( -1 , 0 ) * playerSpeed));

    if(animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
      maxFrames = 16;
      animationFrame++;

      if(animationFrame >= maxFrames)
      {
        animationFrame = 0;
      }

      sprite.setTextureRect(sf::IntRect(
        {animationFrame * 96, Yindex * 96}, {96, 96}));

        animationClock.restart();
    }
  }
  else
  {
    sprite.setTexture( standingTexture );

    if(animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
      maxFrames = 10;
      animationFrame++;

      if(animationFrame >= maxFrames)
      {
        animationFrame = 0;
      }

      sprite.setTextureRect(sf::IntRect(
        {animationFrame * 96, Yindex * 96}, {96, 96}));

        animationClock.restart();
    }
  }
  float minX = 0.f;
    float maxX = 1280.f; // Change this to your map's max width limit

    // Clamp the position so it cannot go past the map edges
    if (position.x < minX)
    {
      position.x = minX;
      sprite.setPosition(position);
    }
    
    if (position.x > maxX)
    {
      position.x = maxX;
      sprite.setPosition(position);
    }
}

void Player::Draw(sf::RenderWindow& window)
{
  window.draw( sprite );
}

sf::Vector2f Player::getPosition()
{
  return sprite.getPosition();
}

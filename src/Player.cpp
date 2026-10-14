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
  attackTexture.loadFromFile("assets/PlayerCharacter/ATTACK 1.png")&&
  runTexture.loadFromFile("assets/PlayerCharacter/RUN.png")&&
  runLeftTexture.loadFromFile("assets/PlayerCharacter/RUN_left.png"))
  {
    std::cout << "Player Image-> Loaded" << std::endl;
  }
  else
  {
    std::cout << "Failed to Load..." << std::endl;
  }
    sprite.setPosition(sf::Vector2f( 50, 450));

    sprite.setTextureRect(sf::IntRect(
      {Xindex * size.x, Yindex * size.y}, {size.x, size.y}));

    sprite.setScale(sf::Vector2f(3,3));
}

void Player::Update()
{
  sf::Vector2f position = sprite.getPosition();


  if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
  {
    attackKeyReleased = true;
  }

  if(!isAttacking&& sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
  {
    isAttacking = true;
    sprite.setTexture( attackTexture );
    attackFrame = 0;
    animationClock.restart();
  }
  if(isAttacking)
  {
    if(animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
      maxFrames = 7;
       
      if (attackFrame <= maxFrames)
      {
        std::cout << "Playing Attack Frame: " << attackFrame << std::endl;
        sprite.setTextureRect(sf::IntRect(
          {attackFrame * 96, Yindex * 96}, {96, 96}));

        attackFrame++;
       animationClock.restart();    
      }
      else if(attackFrame > maxFrames)
      {
        isAttacking = false;
        attackFrame = 0;
        sprite.setTexture(standingTexture);
      }
    }
  }

  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
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
        {animationFrame * 96, Xindex * 96}, {96, 96}));

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

}

void Player::Draw(sf::RenderWindow& window)
{
  window.draw( sprite );
}

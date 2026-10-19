#include "Player.h"
#include <iostream>

Player::Player():
sprite( standingTexture )
{
  playerSpeed = .5;
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
  runTexture.loadFromFile("assets/PlayerCharacter/RUN.png"))
  {
    std::cout << "Player Image-> Loaded" << std::endl;
  }
  else
  {
    std::cout << "Failed to Load-> Movement" << std::endl;
  }


  attackTextures.resize(4); //4 attack variations
  for(int i = 0; i < 4; i++)
  {
    if(i == 3)
    {
      if(attackTextures[i].loadFromFile("assets/PlayerCharacter/AIR-ATTACK.png"))
      {
        std::cout << "Air texture loaded" << std::endl;
      }
      else
      {
        std::cout << "Failed to Load air texture" << std::endl;
      }
    }
      // Loads ATTACK 0.png, ATTACK 1.png, ATTACK 2.png
      if(attackTextures[i].loadFromFile("assets/PlayerCharacter/ATTACK " + std::to_string(i) + ".png"))
      {
        std::cout << "Attacks Loaded" << std::endl;
      }
      else
      {
        std::cout << "Failed to load-> Attacks" << std::endl;
      }
  }

  jumpTextures.resize(3);
  for(int i = 0; i < 3; i++)
  {
      // Loads ATTACK 0.png, ATTACK 1.png, ATTACK 2.png
      if(jumpTextures[i].loadFromFile("assets/PlayerCharacter/JUMP " + std::to_string(i) + ".png"))
      {
        std::cout << "Jump Animations Loaded" << std::endl;
      }
      else
      {
        std::cout << "Failed to load-> Jumps" << std::endl;
      }
  }

    sprite.setPosition(sf::Vector2f( 50, 608));

    sprite.setTextureRect(sf::IntRect(
      {Xindex * size.x, Yindex * size.y}, {size.x, size.y}));
      sprite.setOrigin({96.f / 2.f, 96.f / 2.f});

    sprite.setScale(sf::Vector2f(2, 2));
}

void Player::Update(float deltaTime)
{

  handleAttack();

  // If we are currently attacking, we usually want to lock movement
  if (isAttacking)
  {
        sf::Vector2f position = sprite.getPosition();

    // 1. Maintain horizontal momentum during attacks (Ground or Air)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
      position.x += playerSpeed * deltaTime;

      sprite.setScale(sf::Vector2f(2.f, 2.f));
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
      position.x -= playerSpeed * deltaTime;

      sprite.setScale(sf::Vector2f(-2.f, 2.f));
    }

    if(!isGrounded)
        {
           if(delayClock.getElapsedTime().asSeconds() >= gravityDelay)
          {
            velocityY += gravity * deltaTime;
          }
          sf::Vector2f position = sprite.getPosition();
          position.y += velocityY;
          // --- 4. Ground Collision ---
          // Check if player lands on or passes the ground level
          if (position.y >= groundLevel)
          {
            position.y = groundLevel; // Snap back to the floor
            velocityY = 0.f;          // Stop falling
            isGrounded = true;        // Allow jumping again
          }
        } 
          sprite.setPosition(position);
          return;
    // Clamp position just in case and return early so movement keys don't fire
    //return; 
  }
   // Fetch position once for this frame
  sf::Vector2f position = sprite.getPosition();

  // Pass position by reference (&) so both functions can modify it
  handleJump(position, deltaTime);
  handleInput(position, deltaTime);

  // Apply the final combined position once at the end
  sprite.setPosition(position);

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
    if(isGrounded)
    {
      sprite.setTexture(attackTextures[count]);
    }
    else if(!isGrounded)
    {
      sprite.setTexture(attackTextures[3]);
    }
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


void Player::handleInput(sf::Vector2f& position,float deltaTime)
{
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
  {
    if(isGrounded)
    {    
      sprite.setTexture( runTexture );
    }
    position.x += playerSpeed * deltaTime;

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

        sprite.setScale(sf::Vector2f(2.f, 2.f));

        animationClock.restart();
    }
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
  {
    if(isGrounded)
    {
      sprite.setTexture( runTexture );
    }
    position.x -= playerSpeed * deltaTime;

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

        sprite.setScale(sf::Vector2f(-2.f, 2.f));

        animationClock.restart();
    }
  }
  else if(isGrounded)
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
    }
    
    if (position.x > maxX)
    {
      position.x = maxX;
    }
      sprite.setPosition(position);
}

void Player::handleJump(sf::Vector2f& position, float DeltaTime)
{
   // --- 2. Jumping Logic ---
  // If player presses Space and is currently touching the ground
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded)
  {
    delayClock.restart();
    velocityY = -jumpStrength; // Negative moves up in SFML window coordinates
    isGrounded = false;
    animationFrame = 0;
    animationClock.restart();
  }

  if (!isGrounded)
  {
    // Switch texture based on current animation frame
    if (animationFrame < jumpTextures.size())
    {
      sprite.setTexture(jumpTextures[animationFrame]);
    }

    // Advance frames based on your animation clock timer
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
      animationFrame++;
      if (animationFrame >= jumpTextures.size())
      {
        animationFrame = jumpTextures.size() - 1; // Hold the last jump frame until landing
      }
      animationClock.restart();
    }
    
    // Reset texture rect for the jump files (assuming they are single frames or standard size)
    sprite.setTextureRect(sf::IntRect({0, 0}, {size.x, size.y}));
  }

  // --- 3. Gravity Physics ---
  // Apply gravity to vertical velocity every frame
  if(delayClock.getElapsedTime().asSeconds() >= gravityDelay)
  {
    velocityY += gravity * DeltaTime;
  }
    position.y += velocityY;
  // --- 4. Ground Collision ---
  // Check if player lands on or passes the ground level
  if (position.y >= groundLevel)
  {
    position.y = groundLevel; // Snap back to the floor
    velocityY = 0.f;          // Stop falling
    isGrounded = true;        // Allow jumping again
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

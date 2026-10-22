#include "Player.h"
#include "Enemy.h"
#include <iostream>

Player::Player():
sprite( standingTexture )
{
  playerSpeed = .5;
  healthIndex = health;
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
  runTexture.loadFromFile("assets/PlayerCharacter/RUN.png") && damageTexture.loadFromFile("assets/PlayerCharacter/HURT.png") && dashTexture.loadFromFile("assets/PlayerCharacter/DASH.png"))
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


    //hit box debugging
    debugHitbox.setFillColor(sf::Color::Transparent);
    debugHitbox.setOutlineColor(sf::Color::Red);
    debugHitbox.setOutlineThickness(2.f);
}

void Player::Update(float deltaTime)
{
      if (invincibilityTimer > 0.f)
      {
        invincibilityTimer -= deltaTime / 1000.0f;
      }
  if (hurtTimer > 0.f)
    {
      hurtTimer -= deltaTime / 1000.0f; // Decrement timer based on deltaTime scale

      if (animationClock.getElapsedTime().asSeconds() >= 0.12f) // Adjust speed here (e.g., 0.12 seconds per frame)
      {
          animationFrame++;
          if (animationFrame >= maxFrames)
          {
              animationFrame = maxFrames - 1; // Hold on last frame until hurt ends
          }


          sprite.setTextureRect(sf::IntRect({animationFrame * 96, 0}, {96, 96}));
          animationClock.restart();
      }


      return; // Locks player movement and inputs while hurt animation plays out
    }

  //hit box debugging
sf::FloatRect bounds = getGlobalBounds();
    debugHitbox.setPosition({bounds.position.x, bounds.position.y});
    debugHitbox.setSize({bounds.size.x, bounds.size.y});

  sf::Vector2f position = sprite.getPosition();

  handleDash(position, deltaTime);
    if (isDashing)
    {
      sprite.setPosition(position);
      if(!isGrounded)
      {
        applyGravity( deltaTime );
      }
      return; // Exit early so run/jump/input don't overlap with dash
    }


  if(!isAttacking)
  {
    handleInput(position, deltaTime);
    
  }
  handleAttack();

  // If we are currently attacking, we usually want to lock movement
  if (isAttacking)
  {


    if(playerFacingRight)
    {
        // Spawn hitbox to the right of the player
        playerAttackBox = sf::FloatRect({position.x + 25.f, position.y}, {50.f, 50.f});
    }
    else
    {
        // Spawn hitbox to the left of the player
        playerAttackBox = sf::FloatRect({position.x - 100.f, position.y}, {50.f, 50.f});
    }
    if(!isGrounded)
    {
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
      applyGravity( deltaTime );
    }

    rectangle.setPosition({playerAttackBox.position.x, playerAttackBox.position.y});
    rectangle.setSize({playerAttackBox.size.x, playerAttackBox.size.y});

    // 1. Maintain horizontal momentum during attacks (Ground or Air)
   

    if(!isGrounded)
      {
        applyGravity(deltaTime);
        sprite.setPosition(position);
        return; 
      }
        
    //return;
    // Clamp position just in case and return early so movement keys don't fire
  }
   // Fetch position once for this frame
  
  position = sprite.getPosition();
  handleDash(position, deltaTime);
  handleJump(position, deltaTime);
  
  

  // Apply the final combined position once at the end
  sprite.setPosition(position);

}

void Player::handleAttack()
{
  if (sprite.getScale().x > 0.f)
  {
    playerFacingRight = true;
  }
  else
  {
    playerFacingRight = false;
  }

  // 1. Trigger the attack when K is pressed
  if (!isAttacking && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K))
  {
    isAttacking = true;
    hasDealtDamage = false;
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

void Player::handleDash(sf::Vector2f& position, float deltaTime)
{
    // Trigger dash when 'E' is pressed and player isn't already dashing or attacking
    if (!isDashing && !isAttacking && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
    {
        isDashing = true;
        sprite.setTexture(dashTexture);
        maxFrames = 8; // Adjust to match your DASH.png total frames
        animationFrame = 0;
        animationClock.restart();
    }

    if (isDashing)
    {
        // Burst movement forward based on facing direction, ignoring other keys
        if (playerFacingRight && animationFrame < 6)
        {
            position.x += playerSpeed * 1.5f * deltaTime;
        }
        else if(!playerFacingRight && animationFrame < 6)
        {
            position.x -= playerSpeed * 1.5f * deltaTime;
        }

        if(animationFrame >= 6)
        {
          frameDuration = .01;
        }


        position.x = std::max(0.f, std::min(position.x, 1280.f));

        
        if (animationClock.getElapsedTime().asSeconds() >= frameDuration)
        {
            animationFrame++;
            if (animationFrame < maxFrames)
            {
                sprite.setTextureRect(sf::IntRect({animationFrame * 96, 0}, {96, 96}));
                animationClock.restart();
            }
            else
            {
                // Dash animation complete, return to standing/normal state
                isDashing = false;
                animationFrame = 0;
                maxFrames = 10;
                frameDuration = .1;
                sprite.setTexture(standingTexture);
                sprite.setTextureRect(sf::IntRect({0, 0}, {size.x, size.y}));
                animationClock.restart();
            }
        }
    }
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
  //window.draw(rectangle);
  //window.draw( debugHitbox );
}

sf::Vector2f Player::getPosition()
{
  return sprite.getPosition();
}

sf::FloatRect Player::getAttackBounds() const
{
  if (!isAttacking)
  {
    return sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
  }
  return playerAttackBox;
}

bool Player::hasHitEnemy() const
{
  return hasDealtDamage;
}

void Player::setHitEnemy(bool hit)
{
  hasDealtDamage = hit;
}

bool Player::tookDamage()
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

void Player::decreaseHealth()
{
  if (invincibilityTimer > 0.0f)
  {
      return;
  }
  if(health > 0)
  {
    health -= 50;

    sprite.setTexture( damageTexture );
    maxFrames = 4;
    animationFrame = 0;

    sprite.setTextureRect(sf::IntRect({animationFrame * 96, 0}, {96, 96}));

    hurtTimer = 0.7f;
    invincibilityTimer = .7f;
    animationClock.restart();
  }
}

sf::FloatRect Player::getGlobalBounds() const
{
    sf::Vector2f pos = sprite.getPosition();
    
    // Custom tight box ignoring transparent padding
    return sf::FloatRect(
        {pos.x - 30.f, pos.y}, 
        {60.f, 60.f}                 
    );
}

void Player::applyGravity(float deltaTime)
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

#pragma once
#include <SFML/Graphics.hpp>

class Player
{
  private:
    sf:: Texture standingTexture;
    sf:: Texture runTexture; 
    sf::Texture runLeftTexture;
    std::vector<sf::Texture> attackTextures;
    std::vector<sf::Texture> jumpTextures;

    sf::Vector2i size;
    sf::Vector2f position;

    float playerSpeed;

    sf::Vector2f playerPositiion;

    float velocityY = 0.f;
    float gravity = 0.035f;        // Adjust for heavier/lighter fall feel
    float jumpStrength = 10.f;   // Adjust for higher/lower jumps
    bool isGrounded = true;
    float groundLevel = 608.f;   // Where your floor currently sits
    float gravityDelay = .02;

    sf::Clock animationClock;
    sf::Clock delayClock;
  public:
    sf::Sprite sprite;

    int count = -1;
    int Xindex = 0;
    int Yindex = 0;

    int animationFrame = 0;
    int attackFrame = 0;
    int maxFrames = 10;
    float frameDuration = .1f;
    float attackFrameDuration = .05f;
    bool isAttacking = false;

  public:
    Player();
    ~Player();

    void Initialize();

    void Load();

    void Update(float deltaTime);

    void handleAttack();

    void handleInput(sf::Vector2f& position, float deltaTime);

    void handleJump(sf::Vector2f& position, float deltaTime);

    void Draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();
};
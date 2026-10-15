#pragma once
#include <SFML/Graphics.hpp>

class Player
{
  private:
    sf:: Texture standingTexture;
    sf:: Texture runTexture; 
    sf::Texture runLeftTexture;
    std::vector<sf::Texture> attackTextures;

    sf::Vector2i size;

    float playerSpeed;

    sf::Vector2f playerPositiion;

  public:
    sf::Sprite sprite;

    int count = -1;
    int Xindex = 0;
    int Yindex = 0;

    sf::Clock animationClock;
    int animationFrame = 0;
    int attackFrame = 0;
    int maxFrames = 10;
    float frameDuration = .1f;
    float attackFrameDuration = .08;
    bool isAttacking;

  public:
    Player();
    ~Player();

    void Initialize();

    void Load();

    void Update();

    void handleAttack();

    void Draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();
};
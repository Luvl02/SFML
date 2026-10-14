#pragma once
#include <SFML/Graphics.hpp>

class Player
{
  private:
    sf:: Texture standingTexture;
    sf:: Texture attackTexture;
    sf:: Texture runTexture; 
    sf::Texture runLeftTexture;

    sf::Vector2i size;

    float playerSpeed;
  public:
    sf::Sprite sprite;

    int Xindex = 0;
    int Yindex = 0;

    sf::Clock animationClock;
    int animationFrame = 0;
    int attackFrame = 0;
    int maxFrames = 10;
    float frameDuration = .1f;
    bool isAttacking = false;
    bool attackKeyReleased;

  public:
    Player();
    ~Player();

    void Initialize();

    void Load();

    void Update();

    void Draw(sf::RenderWindow& window);
};
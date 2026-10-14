#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Map
{
  private:
    static const int numOfFiles = 12;
    std::vector <sf::Texture> texture;
    std::vector <sf::Sprite> sprite;
  public:
    Map();

    ~Map();

   void Load(sf::RenderWindow& window);

    void Draw(sf::RenderWindow& window);
  };
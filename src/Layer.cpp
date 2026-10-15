#include "Layer.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::Load(sf::RenderWindow& window)
{
  texture.resize(numOfFiles);
  sprite.clear();

  for(int i = 0; i < numOfFiles; i++)
  {
    if (texture[i].loadFromFile("assets/MapLayers/"+std::to_string(i) + ".png"))
    {
      std::cout << "Map Texture " << i << " Loaded" << std::endl;
    }
    
    // Link the sprite to the loaded texture
    sprite.emplace_back(texture[i]);
    sf::Vector2u textureSize = texture[i].getSize();
      
      // Calculate scale factors for X and Y to match the window
      float scaleX = (float)window.getSize().x/(float)(textureSize.x);
      float scaleY = (float)window.getSize().y / (float)(textureSize.y);
      
      // Apply the scale
      sprite.back().setScale(sf::Vector2f(scaleX, scaleY));
  }
  
}

void Map::Draw(sf::RenderWindow& window)
{

  for(int i = numOfFiles-1; i >= 1; i--)
  {
    window.draw( sprite[i] );
  }
}

void Map::drawLast(sf::RenderWindow &window)
{
  window.draw( sprite[0] );
}

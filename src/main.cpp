#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Layer.h"
#include "Sanzoku.h"

using namespace std;
using namespace sf;

int main()
{
/*Init Window*/
 ContextSettings settings;
 settings.antiAliasingLevel = 8;

 RenderWindow window(VideoMode({1280, 720}), "Ninjas", Style::Default, State::Windowed, settings);

 sf::View camera(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));

 window.setFramerateLimit(60);
 /*Init Window*/

 /*Initilize*/
 Map map;
 Player player;
 Sanzoku sanzoku(1000, 608);
 player.Initialize();
 /*Initilize*/

 vector<unique_ptr<Enemy>> enemies;

 Clock spawnClock;
 float nextSpawn = 3.5f;

 /*Load*/
 map.Load( window );
 player.Load();

 sanzoku.Load();
 /*Load*/

Clock clock;
 /*Game Loop*/
 while(window.isOpen())
 {
   Time deltaTimeCount = clock.restart();
    double deltaTime = deltaTimeCount.asMicroseconds() / 1000.0;
    while(const auto event = window.pollEvent())
    {
      if(event->is<Event::Closed>())
      {
        window.close();
      }
    }

    if (spawnClock.getElapsedTime().asSeconds() >= nextSpawn)
    {
        float spawnX = (rand() % 2 == 0) ? 50.f : 1200.f;
        
        auto newEnemy = std::make_unique<Sanzoku>(spawnX, 608.f);

        newEnemy->Load();

        enemies.push_back(std::move(newEnemy));

        spawnClock.restart();

        nextSpawn = 3.0f + static_cast<float>(rand() % 4000) / 1000.0f;
    }
    /*Update*/
    player.Update(deltaTime);
    player.handleDamage(sanzoku);

   for (auto& enemy : enemies)
   {
    if(!enemy->isDead())
    {
      enemy->Update(deltaTime, player.getPosition());

      enemy->handleDamage(player);
    }
   }
   enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(), [](const auto& enemy) {
        return enemy->isDead();
    }),
    enemies.end()
);
    /*Update*/


    /*Draw*/
    window.clear(Color::Black);

    map.Draw( window );
   
   for(auto& enemy : enemies)
   {
    if(!enemy->isDead())
    {
      enemy->Draw(window);
    }
   }
    player.Draw( window );
    map.drawLast( window );
    window.display();
    /*Draw*/
    }
}
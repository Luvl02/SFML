#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Layer.h"
#include "Sanzoku.h"

using namespace std;
using namespace sf;

int main()
{
  std::filesystem::current_path();
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
    /*Update*/
    player.Update(deltaTime);

    if(!sanzoku.isDead())
    {

     sf::FloatRect sanzokuAttack = sanzoku.getAttackBounds();
    if (sanzokuAttack.size.x > 0 && sanzokuAttack.findIntersection(player.getGlobalBounds()))
    {
      
      if (!sanzoku.hasHitPlayer() && sanzoku.attackAnimationFinished)
        {
            player.decreaseHealth();
            sanzoku.setHitPlayer(true); // Mark this swing as having landed a hit
            
            cout << player.health << endl;
            std::cout << "Hit Player!" << std::endl;
        }
        // TODO: Trigger enemy damage or knockback here!
    }
    }

    if(!sanzoku.isDead())
    {
    sanzoku.Update( deltaTime, player.getPosition());

     sf::FloatRect playerAttack = player.getAttackBounds();
    if (playerAttack.size.x > 0 && playerAttack.findIntersection(sanzoku.getGlobalBounds()))
    {
      
      if (!player.hasHitEnemy())
        {
            sanzoku.decreaseHealth();
            player.setHitEnemy(true); // Mark this swing as having landed a hit
            
            cout << sanzoku.health << endl;
            std::cout << "Hit Sanzoku!" << std::endl;
        }
        // TODO: Trigger enemy damage or knockback here!
    }
    }
    /*Update*/


    /*Draw*/
    window.clear(Color::Black);

    map.Draw( window );
    if(!sanzoku.isDead())
    {
      sanzoku.Draw( window );
    }
    player.Draw( window );
    map.drawLast( window );
    window.display();
    /*Draw*/
    }
}
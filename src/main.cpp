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
    sanzoku.Update( deltaTime, player.getPosition());
    /*Update*/


    /*Draw*/
    window.clear(Color::Black);

    map.Draw( window );
    sanzoku.Draw( window );
    player.Draw( window );
    map.drawLast( window );
    window.display();
    /*Draw*/
    }
}
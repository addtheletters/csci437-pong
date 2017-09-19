#include <memory>
#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Ball.h"
#include "Paddle.h"

int main(int argc, char** argv)
{
  // create main window
  sf::RenderWindow App(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML");
 
  // constants
  sf::Color background(30, 100, 240);

  // testing some stuff
  sf::RectangleShape box;
  box.setSize(sf::Vector2f(100, 200));
  box.setFillColor(sf::Color::Red);
  box.setOrigin(sf::Vector2f(50, 50));

  box.setPosition(sf::Vector2f(400, 300));
  
  // set up clock for delta time
  sf::Clock frame_timer;
  sf::Time last_frame;
  sf::Time delta;
  
  // set up map for entities
  ENTITY_MAP entities;
  
  // add entities
  entities["ball"]  = std::unique_ptr<Entity>(new Ball(50));
  entities["p1"]    = std::unique_ptr<Entity>(new Paddle(sf::Vector2f(50, 300), sf::Vector2f(300, 300)));
  
  entities["ball"]->setPos(sf::Vector2f(100,100));
  
  // start main loop
  while(App.isOpen())
  {
    delta = frame_timer.getElapsedTime() - last_frame;
    last_frame = frame_timer.getElapsedTime();
    
    // process events
    sf::Event Event;
    while(App.pollEvent(Event))
    {
      // Exit
      if(Event.type == sf::Event::Closed)
        App.close();
      if(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)
        App.close();
      
      if(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::W){
        std::cout << "oh" << std::endl;
        entities["ball"]->setVel(sf::Vector2f(1000.0f, 0));
      }
    }
    
    // update entities
    //std::cout << "elapsed time "<< last_frame.asSeconds() << ", delta time: " << delta.asMicroseconds() << std::endl;
    for(auto it = entities.begin(); it != entities.end(); it++){
        //std::cout << "Updating " << it->first << std::endl;
        it->second->tick(delta, entities, App.getSize());
    }
    
    // clear screen and fill with blue
    App.clear(background);

    App.draw(box);
    
    // draw entities
    for(auto it = entities.begin(); it != entities.end(); it++){
        it->second->draw(App);
    }

    // display
    App.display();
  }

  // Done.
  return 0;
}

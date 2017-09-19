#include <memory>
#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Ball.h"
#include "Paddle.h"

Paddle* getPaddle(ENTITY_MAP& entities, std::string id){
    return dynamic_cast<Paddle*>(entities["p1"].get());
}

int main(int argc, char** argv) {
    
    const float PADDLE_MOVE_SPEED = 300.0f;
    
    // create main window
    sf::RenderWindow App(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML");
    
    // constants
    sf::Color background(30, 100, 240);
    
    // set up clock for delta time
    sf::Clock frame_timer;
    sf::Time last_frame;
    sf::Time delta;
    
    // set up map for entities
    ENTITY_MAP entities;
    
    // add entities
    entities["ball"]  = std::unique_ptr<Entity>(new Ball(20));
    entities["p1"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(100, 30), sf::Vector2f(400, 510)));
    
    entities["ball"]->setPos(sf::Vector2f(100,100));
    
    // start main loop
    while (App.isOpen()) {
        delta = frame_timer.getElapsedTime() - last_frame;
        last_frame = frame_timer.getElapsedTime();
        
        // process events
        sf::Event Event;
        while (App.pollEvent(Event)) {
            // Exit
            if(Event.type == sf::Event::Closed)
                App.close();
            if(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)
                App.close();
            
            if(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::W){
                std::cout << "oh" << std::endl;
                entities["ball"]->setVel(sf::Vector2f(500, 300));
            }
        }
        
        // read non-event input
        // move player paddle horizontally
        bool left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        if (left_pressed || right_pressed){
            float multiplier = 0;
            if (right_pressed)
                multiplier += 1;
            if (left_pressed){
                multiplier -= 1;
            }
            if (getPaddle(entities, "p1")->tryMoveHorizontal(
                multiplier * delta.asSeconds() * PADDLE_MOVE_SPEED, App.getSize())){
                std::cout << "Paddle moved." << std::endl;
            }
            else{
                std::cout << "Paddle hit something." << std::endl;
            }
        }
        
        // update entities
        /*std::cout << "elapsed time "<< last_frame.asSeconds() 
        << ", delta time: " << delta.asMicroseconds() << std::endl;*/
        for (auto it = entities.begin(); it != entities.end(); it++) {
            //std::cout << "Updating " << it->first << std::endl;
            it->second->tick(delta, entities, App.getSize());
        }
        
        // clear screen and fill with background color
        App.clear(background);
        
        // draw entities
        for (auto it = entities.begin(); it != entities.end(); it++) {
            it->second->draw(App);
        }

        // display
        App.display();
    }

    // Done.
    return 0;
}

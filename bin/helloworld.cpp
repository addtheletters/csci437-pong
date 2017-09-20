#include <memory>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Ball.h"
#include "Paddle.h"
#include "AutoPlayer.h"

Paddle* getPaddle(ENTITY_MAP& entities, std::string id){
    return dynamic_cast<Paddle*>(entities[id].get());
}

int main(int argc, char** argv) {
    
    // constants
    sf::Color background(30, 100, 240);
    const float PADDLE_MOVE_SPEED = 500.0f;
    const float BOUNCE_RANDOMNESS = 0.1f;
    
    // seed RNG
    std::srand(1234);
    
    // create main window
    sf::RenderWindow App(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML");
    
    // set up clock for delta time
    sf::Clock frame_timer;
    sf::Time last_frame;
    sf::Time delta;
    
    // create AI controller
    std::unique_ptr<AutoPlayer> ai(new AutoPlayer());
    
    // set up map for entities
    ENTITY_MAP entities;
    
    // add entities
    entities["ball"]  = std::unique_ptr<Entity>(new Ball(20));
    entities["ball"]->setPos(sf::Vector2f(100,100));
    entities["p1"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(100, 30), sf::Vector2f(400, 550)));
    entities["p2"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(100, 30), sf::Vector2f(400, 50)));
    
    // start main loop
    while (App.isOpen()) {
        // calculate delta time
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
                std::cout << "Boop." << std::endl;
                entities["ball"]->setVel(std::rand() % 1000, 2 * 3.1415 * (double)(std::rand() % 360) / 360);
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
                //std::cout << "Paddle moved." << std::endl;
            }
            else{
                //std::cout << "Paddle hit a wall." << std::endl;
            }
        }
        
        // move AI-controlled paddle horizontally
        AutoPlayer::Action ai_action = 
            ai->getAIAction(App.getSize(), 
                            entities["ball"]->getPos(), 
                            entities["ball"]->getVel(), 
                            entities["p2"]->getPos(),
                            1.0f,
                            5.0f
                           );
        if (ai_action != AutoPlayer::Action::NONE){
            float multiplier = 0;
            if (ai_action == AutoPlayer::Action::MOVE_RIGHT)
                multiplier = 1;
            if (ai_action == AutoPlayer::Action::MOVE_LEFT)
                multiplier = -1;
            if (getPaddle(entities, "p2")->tryMoveHorizontal(
                multiplier * delta.asSeconds() * PADDLE_MOVE_SPEED, App.getSize())){
                //std::cout << "Paddle moved." << std::endl;
            }
            else{
                //std::cout << "Paddle hit a wall." << std::endl;
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

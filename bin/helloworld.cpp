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

void applyPaddleInput(Paddle* paddle, float multiplier, sf::Time delta){
    std::cout << "mult " << multiplier << std::endl;
    if (multiplier == 0){
        paddle->setVel(sf::Vector2f(0,0));
        return;
    }
    // if moving in opposite direction, reset velocity first
    if (multiplier * paddle->getVel().y <= 0){
        paddle->setVel(sf::Vector2f(0, 200.0f * multiplier));
    }
    
    float new_paddle_vy = std::max(-1000.0f, std::min(1000.0f, paddle->getVel().y + multiplier * delta.asSeconds() * 1500.0f));
    // apply acceleration
    paddle->setVel(sf::Vector2f(0, new_paddle_vy));
}

int main(int argc, char** argv) {
    
    // constants
    sf::Color background(30, 100, 240);
    const float PADDLE_MOVE_SPEED_MAX = 1000.0f;
    const float PADDLE_MOVE_SPEED_BASE = 200.0f;
    const float PADDLE_MOVE_ACCEL = 1500.0f;
    const float PADDLE_HEIGHT = 120.0f;
    const float PADDLE_THICKNESS = 20.0f;
    const float PADDLE_BACK_DISTANCE = 30.0f;
    const float BALL_START_SPEED = 300.0f;
    
    // seed RNG
    std::srand(14315);
    
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
    entities["ball"]->setPos(sf::Vector2f(App.getSize().x / 2, App.getSize().y / 2));
    
    float start_y_vel = ((std::rand() % 7) - 3.5) * BALL_START_SPEED;
    float start_x_vel = ((std::rand() % 2) * 2 - 1) * BALL_START_SPEED;
    entities["ball"]->setVel(sf::Vector2f(start_x_vel, start_y_vel));
    
    entities["p1"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT),
                   sf::Vector2f(PADDLE_BACK_DISTANCE, App.getSize().y / 2)));
    entities["p2"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT), 
                   sf::Vector2f(App.getSize().x - PADDLE_BACK_DISTANCE, App.getSize().y / 2)));
    
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
                dynamic_cast<Ball*>(entities["ball"].get())->boop();
            }
        }
        
        // read non-event input
        // move player paddle horizontally
        bool up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        float human_move_multiplier = 0;
        if (down_pressed)
            human_move_multiplier += 1;
        if (up_pressed){
            human_move_multiplier -= 1;
        }
        applyPaddleInput(getPaddle(entities, "p1"), human_move_multiplier, delta);
    
        // move AI-controlled paddle horizontally
        AutoPlayer::Action ai_action = ai->getAIAction(App.getSize(), 
                                                        entities["ball"]->getPos(), 
                                                        entities["ball"]->getVel(), 
                                                        entities["p2"]->getPos(),
                                                        -1.f,
                                                        5.0f);
        float ai_move_multiplier = 0;
        if (ai_action == AutoPlayer::Action::MOVE_DOWN)
            ai_move_multiplier = 1;
        if (ai_action == AutoPlayer::Action::MOVE_UP)
            ai_move_multiplier = -1;
        applyPaddleInput(getPaddle(entities, "p2"), ai_move_multiplier, delta);
        
        // update entities
        /*std::cout << "elapsed time "<< last_frame.asSeconds() 
        << ", delta time: " << delta.asMicroseconds() << std::endl;*/
        for (auto it = entities.begin(); it != entities.end(); it++) {
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
    
    // clean up?

    // Done.
    return 0;
}

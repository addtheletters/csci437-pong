//
//
//

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
#include "VectorUtility.h"

// constants
const float PADDLE_MOVE_SPEED_MAX  = 1000.0f;
const float PADDLE_MOVE_SPEED_BASE = 200.0f;
const float PADDLE_MOVE_ACCEL      = 1500.0f;
const float PADDLE_HEIGHT          = 120.0f;
const float PADDLE_THICKNESS       = 20.0f;
const float PADDLE_BACK_DISTANCE   = 30.0f;
const float BALL_START_SPEED       = 300.0f;
const float BALL_START_ANGLE_RANGE = 1.7;
const sf::Color BACKGROUND(30, 100, 240);

// helper functions
namespace {

void applyPaddleInput(ENTITY_MAP& entities, std::string paddle_id,
                      float multiplier, sf::Time delta){
    Paddle* paddle = dynamic_cast<Paddle*>(entities[paddle_id].get());
    if (multiplier == 0){
        paddle->setVel(sf::Vector2f(0,0));
        return;
    }
    // if moving in opposite direction, reset velocity first
    if (multiplier * paddle->getVel().y <= 0){
        paddle->setVel(sf::Vector2f(0, PADDLE_MOVE_SPEED_BASE * multiplier));
    }
    // apply acceleration
    float new_paddle_vy = vecutil::clamp(
        paddle->getVel().y + multiplier * delta.asSeconds() * PADDLE_MOVE_ACCEL,
        -PADDLE_MOVE_SPEED_MAX, PADDLE_MOVE_SPEED_MAX);
    paddle->setVel(sf::Vector2f(0, new_paddle_vy));
}

void resetEntities(ENTITY_MAP& entities, sf::Vector2u window_size){
    entities.clear();
    
    // set up ball
    entities["ball"]  = std::unique_ptr<Entity>(
        new Ball(20, sf::Vector2f(window_size.x / 2, window_size.y / 2)));
    // ball always starts heading towards the computer
    float ball_start_angle = BALL_START_ANGLE_RANGE * (((std::rand() % 1000) / 1000.0) - 0.5);
    //float start_y_vel = ((std::rand() % 7) - 3.5) * BALL_START_SPEED;
    //float start_x_vel = ((std::rand() % 2) * 2 - 1) * BALL_START_SPEED;
    entities["ball"]->setVel(vecutil::fromPolar(BALL_START_SPEED, ball_start_angle));
    
    // set up paddles
    entities["p1"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT),
                   sf::Vector2f(PADDLE_BACK_DISTANCE, window_size.y / 2)));
    entities["p2"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT), 
                   sf::Vector2f(window_size.x - PADDLE_BACK_DISTANCE,
                                window_size.y / 2)));
}
    
} // namespace

int main(int argc, char** argv) {
    
    // seed RNG
    std::srand(14315);
    
    // create main window
    sf::RenderWindow App(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML");
    
    // set up clock for delta time
    sf::Clock frame_timer;
    sf::Time last_frame;
    sf::Time delta;
    
    // track score
    int p1_score;
    int p2_score;
    
    // create AI controller
    std::unique_ptr<AutoPlayer> ai(new AutoPlayer(-1.0f, 2.0f));
    
    // set up map for entities
    ENTITY_MAP entities;
    
    resetEntities(entities, App.getSize());
    
    // game loop
    while (App.isOpen()) {
        // calculate delta time
        delta = frame_timer.getElapsedTime() - last_frame;
        last_frame = frame_timer.getElapsedTime();
        
        // process events
        sf::Event Event;
        while (App.pollEvent(Event)) {
            // Exit
            if (Event.type == sf::Event::Closed)
                App.close();
            
            if (Event.type == sf::Event::KeyPressed) {
                // Escape key exits as well
                if (Event.key.code == sf::Keyboard::Escape)
                    App.close();
            
                // Spacebar resets
                if (Event.key.code == sf::Keyboard::Space) {
                    resetEntities(entities, App.getSize());
                }
                
                // W nudges the ball slightly
                if(Event.key.code == sf::Keyboard::W){
                    dynamic_cast<Ball*>(entities["ball"].get())->boop();
                }
            }
        }
        
        // read non-event input
        // move player paddle
        float human_move_multiplier = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            human_move_multiplier += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            human_move_multiplier -= 1;
        }
        applyPaddleInput(entities, "p1", human_move_multiplier, delta);
    
        // move AI-controlled paddle
        AutoPlayer::Action ai_action =
            ai->getAIAction(App.getSize(), 
                            entities["ball"]->getPos(), 
                            entities["ball"]->getVel(), 
                            entities["p2"]->getPos());
        float ai_move_multiplier = 0;
        if (ai_action == AutoPlayer::Action::MOVE_DOWN)
            ai_move_multiplier = 1;
        if (ai_action == AutoPlayer::Action::MOVE_UP)
            ai_move_multiplier = -1;
        applyPaddleInput(entities, "p2", ai_move_multiplier, delta);
        
        // update entities
        /*std::cout << "elapsed time "<< last_frame.asSeconds() 
        << ", delta time: " << delta.asMicroseconds() << std::endl;*/
        for (auto it = entities.begin(); it != entities.end(); it++) {
            it->second->tick(delta, entities, App.getSize());
        }
        
        // clear screen and fill with background color
        App.clear(BACKGROUND);
        
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

#include "PongGame.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Ball.h"
#include "VectorUtility.h"

// ---------- Constant definitions

const float PongGame::PADDLE_MOVE_SPEED_MAX  = 1000.0f;
const float PongGame::PADDLE_MOVE_SPEED_BASE = 200.0f;
const float PongGame::PADDLE_MOVE_ACCEL      = 1500.0f;
const float PongGame::PADDLE_HEIGHT          = 120.0f;
const float PongGame::PADDLE_THICKNESS       = 20.0f;
const float PongGame::PADDLE_BACK_DISTANCE   = 30.0f;
const float PongGame::BALL_START_SPEED       = 300.0f;
const float PongGame::BALL_START_ANGLE_RANGE = 1.7;
const sf::Color PongGame::BACKGROUND(30, 100, 240);

// ------

void PongGame::init(){
    // seed the RNG
    std::srand(14315);
    
    // create main window
    window_ = std::unique_ptr<sf::RenderWindow>(
        new sf::RenderWindow(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML"));
    
    // create AI controller
    ai_ = std::unique_ptr<AutoPlayer>(new AutoPlayer(-1.0f, 2.0f));
    
    reset();
}

void PongGame::reset(){
    resetEntities();
    resetScore();
}

int PongGame::gameLoop(){
    while (window_->isOpen()) {
        // calculate delta time
        delta_ = frame_timer_.getElapsedTime() - last_frame_;
        last_frame_ = frame_timer_.getElapsedTime();
        
        // process events
        sf::Event evnt;
        while (window_->pollEvent(evnt)) {
            // Exit
            if (evnt.type == sf::Event::Closed)
                window_->close();
            
            if (evnt.type == sf::Event::KeyPressed) {
                // Escape key exits as well
                if (evnt.key.code == sf::Keyboard::Escape)
                    window_->close();
            
                // Spacebar resets
                if (evnt.key.code == sf::Keyboard::Space) {
                    reset();
                }
                
                // W nudges the ball slightly
                if (evnt.key.code == sf::Keyboard::W){
                    dynamic_cast<Ball*>(entities_["ball"].get())->boop();
                }
                
                // TODO remove this temp testing stuff
                if (evnt.key.code == sf::Keyboard::Num1){
                    scorer_ = 1;
                }
                if (evnt.key.code == sf::Keyboard::Num2){
                    scorer_ = 2;
                }
            }
        }
        
        // check if anyone has scored
        if (scorer_ != 0){
            switch (scorer_){
                case 1:
                    p1_score_++;
                    break;
                case 2:
                    p2_score_++;
                    break;
                default:
                    break;
            }
            std::cout << "Player " << scorer_ << " scored." << std::endl;
            resetEntities();
            scorer_ = 0;
        }
        
        // read non-event input
        // move player paddle
        float human_move_multiplier = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            human_move_multiplier += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            human_move_multiplier -= 1;
        }
        applyPaddleInput("p1", human_move_multiplier);
    
        // move AI-controlled paddle
        AutoPlayer::Action ai_action =
            ai_->getAIAction(window_->getSize(), 
                            entities_["ball"]->getPos(), 
                            entities_["ball"]->getVel(), 
                            entities_["p2"]->getPos());
        float ai_move_multiplier = 0;
        if (ai_action == AutoPlayer::Action::MOVE_DOWN)
            ai_move_multiplier = 1;
        if (ai_action == AutoPlayer::Action::MOVE_UP)
            ai_move_multiplier = -1;
        applyPaddleInput("p2", ai_move_multiplier);
        
        // update entities
        /*std::cout << "elapsed time "<< last_frame.asSeconds() 
        << ", delta time: " << delta.asMicroseconds() << std::endl;*/
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->tick(delta_, entities_, window_->getSize());
        }
        
        // clear screen and fill with background color
        window_->clear(BACKGROUND);
        
        // draw entities
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->draw(*window_);
        }

        // display
        window_->display();
    }
    
    // Done.
    return 0;
}

void PongGame::resetEntities() {
    // delete current entities
    entities_.clear();
    sf::Vector2u window_size = window_->getSize();
    
    // set up ball
    entities_["ball"]  = std::unique_ptr<Entity>(
        new Ball(20, sf::Vector2f(window_size.x / 2, window_size.y / 2)));
    // ball always starts heading towards the AI-paddle
    float ball_start_angle = BALL_START_ANGLE_RANGE
                             * (((std::rand() % 1000) / 1000.0) - 0.5);
    entities_["ball"]->setVel(vecutil::fromPolar(BALL_START_SPEED,
                                                 ball_start_angle));
    
    // set up paddles
    entities_["p1"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT),
                   sf::Vector2f(PADDLE_BACK_DISTANCE, window_size.y / 2)));
    entities_["p2"]    = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT), 
                   sf::Vector2f(window_size.x - PADDLE_BACK_DISTANCE,
                                window_size.y / 2)));
}

void PongGame::resetScore() {
    scorer_ = 0;
    p1_score_ = 0;
    p2_score_ = 0;
}

void PongGame::applyPaddleInput(std::string paddle_id, float multiplier){
    Paddle* paddle = dynamic_cast<Paddle*>(entities_[paddle_id].get());
    if (multiplier == 0){
        paddle->setVel(sf::Vector2f(0,0));
        return;
    }
    // if going in opposite, or unmoving, reset to base paddle speed first
    if (multiplier * paddle->getVel().y <= 0){
        paddle->setVel(sf::Vector2f(0, PADDLE_MOVE_SPEED_BASE * multiplier));
    }
    // apply acceleration, limited by max paddle speed
    float new_paddle_vy = vecutil::clamp(
        paddle->getVel().y + multiplier * delta_.asSeconds() * PADDLE_MOVE_ACCEL,
        -PADDLE_MOVE_SPEED_MAX, PADDLE_MOVE_SPEED_MAX);
    paddle->setVel(sf::Vector2f(0, new_paddle_vy));
}

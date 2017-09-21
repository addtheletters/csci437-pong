#include "PongGame.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Ball.h"
#include "VectorUtility.h"

// ---------- Constant definitions

const float PongGame::PADDLE_MOVE_SPEED_MAX  = 1100.0f;
const float PongGame::PADDLE_MOVE_SPEED_BASE = 200.0f;
const float PongGame::PADDLE_MOVE_ACCEL      = 2000.0f;
const float PongGame::PADDLE_HEIGHT          = 120.0f;
const float PongGame::PADDLE_THICKNESS       = 20.0f;
const float PongGame::PADDLE_BACK_DISTANCE   = 30.0f;
const float PongGame::BALL_START_SPEED       = 300.0f;
const float PongGame::BALL_START_ANGLE_RANGE = 1.7;
const int PongGame::SCORE_FONT_SIZE   = 40;
const int PongGame::TITLE_FONT_SIZE   = 70;
const int PongGame::CAPTION_FONT_SIZE = 30;
const int PongGame::POINTS_TO_WIN     = 11;
const std::string PongGame::TITLE = "pong, kinda";
const std::string PongGame::FONT_FILENAME    = "../resources/Inconsolata-Regular.ttf";
const sf::Color PongGame::BACKGROUND(30, 100, 240);

// --------------------

void PongGame::init() {
    // seed the RNG
    std::srand(14315);
    
    // create main window
    window_ = std::unique_ptr<sf::RenderWindow>(
        new sf::RenderWindow(sf::VideoMode(800,600,32), "Ben Zhang's Pong - SFML"));
    
    // create AI controller
    ai_ = std::unique_ptr<AutoPlayer>(new AutoPlayer(-1.0f, 2.0f));
    
    // load display font
    if (!font_.loadFromFile(FONT_FILENAME)){
        std::cout << "Is " << FONT_FILENAME 
                  << " in the same directory as the executable?" << std::endl;
    }
    
    // TODO set up certain dynamic text elements
    
    reset();
}

void PongGame::reset() {
    resetEntities();
    resetScore();
}

int PongGame::gameLoop() {
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
                
                // TODO remove this temp testing stuff
                // W nudges the ball slightly, speeding it up
                // and tweaking its trajectory like when it bounces
                if (evnt.key.code == sf::Keyboard::W){
                    dynamic_cast<Ball*>(entities_["ball"].get())->boop();
                }
                if (evnt.key.code == sf::Keyboard::Num1){
                    scorer_ = 1;
                }
                if (evnt.key.code == sf::Keyboard::Num2){
                    scorer_ = 2;
                }
                if (evnt.key.code == sf::Keyboard::Q){
                    std::cout << "p1 speed: " << entities_["p1"]->getVel().y << std::endl;
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
            
            std::cout << "Score: p1-" << p1_score_ << " ; p2-" << p2_score_ << std::endl;
    
            
            // check if anyone has enough points to win
            int winner = 0;
            if (p1_score_ >= POINTS_TO_WIN) {
                winner = 1;
            }
            else if (p2_score_ >= POINTS_TO_WIN){
                winner = 2;
            }
            if (winner){
                std::cout << "Player " << winner << " wins!" << std::endl;
                // TODO win stuff
                paused_ = true;
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
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->tick(delta_, entities_, window_->getSize());
        }
        
        // clear screen and fill with background color
        window_->clear(BACKGROUND);
        
        // draw score counters
        drawScore();
        
        // draw entities
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->draw(*window_);
        }
        
        // draw menu
        if (paused_) {
            drawMenu();
        }
        
        // display
        window_->display();
    }
    
    // Done.
    return 0;
}

void PongGame::playerScored(int scorer) {
    // Set this flag, which we check inside our game loop.
    scorer_ = scorer;
}

void PongGame::centerTextOrigin(sf::Text& text){
    sf::FloatRect text_bounds = text.getLocalBounds();
    text.setOrigin(text_bounds.left + text_bounds.width/2,
                   text_bounds.top + text_bounds.height/2);
}

void PongGame::showMessage(std::string message){
    // TODO this
}

void PongGame::resetEntities() {
    // delete current entities
    entities_.clear();
    sf::Vector2u window_size = window_->getSize();
    
    // set up ball
    entities_["ball"] = std::unique_ptr<Entity>(
        new Ball(20, sf::Vector2f(window_size.x / 2, window_size.y / 2),
                 (ScoreInterface*)this));
    // ball always starts heading towards the AI-paddle
    float ball_start_angle = BALL_START_ANGLE_RANGE
                             * (((std::rand() % 1000) / 1000.0) - 0.5);
    entities_["ball"]->setVel(vecutil::fromPolar(BALL_START_SPEED,
                                                 ball_start_angle));
    
    // set up paddles
    entities_["p1"] = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT),
                   sf::Vector2f(PADDLE_BACK_DISTANCE, window_size.y / 2)));
    entities_["p2"] = std::unique_ptr<Entity>(
        new Paddle(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT), 
                   sf::Vector2f(window_size.x - PADDLE_BACK_DISTANCE,
                                window_size.y / 2)));
}

void PongGame::resetScore() {
    scorer_ = 0;
    p1_score_ = 0;
    p2_score_ = 0;
}

void PongGame::drawMenu() {
    // draw big title
    sf::Vector2f half_window(window_->getSize().x / 2, window_->getSize().y / 2);
    
    sf::Text title_text(TITLE, font_);
    title_text.setCharacterSize(TITLE_FONT_SIZE);
    title_text.setStyle(sf::Text::Bold);
    centerTextOrigin(title_text);
    title_text.setPosition(sf::Vector2f(half_window.x,
                                        half_window.y - 100));
    
    window_->draw(title_text);
    
}

void PongGame::drawScore() {
    sf::Vector2f half_window(window_->getSize().x / 2, window_->getSize().y / 2);
    
    sf::RectangleShape divider_line(sf::Vector2f(1, window_->getSize().y));
    divider_line.setPosition(sf::Vector2f(half_window.x, 0));
    window_->draw(divider_line);
    
    sf::Text p1_score_text(std::to_string(p1_score_), font_);
    p1_score_text.setCharacterSize(SCORE_FONT_SIZE);
    p1_score_text.setStyle(sf::Text::Bold);
    centerTextOrigin(p1_score_text);
    p1_score_text.setPosition(sf::Vector2f(half_window.x - half_window.x/2,
                                           half_window.y));
    window_->draw(p1_score_text);
    
    sf::Text p2_score_text(std::to_string(p2_score_), font_);
    p2_score_text.setCharacterSize(SCORE_FONT_SIZE);
    p2_score_text.setStyle(sf::Text::Bold);
    centerTextOrigin(p2_score_text);
    p2_score_text.setPosition(sf::Vector2f(half_window.x + half_window.x/2,
                                           half_window.y));
    window_->draw(p2_score_text);
}

void PongGame::applyPaddleInput(std::string paddle_id, float multiplier) {
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

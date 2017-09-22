#include "PongGame.h"

#include <iostream>
#include <iomanip>
#include <sstream>

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

const float PongGame::NOTIF_LIFETIME = 2.0f;
const float PongGame::NOTIF_FADE_RATE = 1.0f;

const int PongGame::SCORE_FONT_SIZE     = 40;
const int PongGame::TITLE_FONT_SIZE     = 70;
const int PongGame::CAPTION_FONT_SIZE   = 30;
const int PongGame::TUTORIAL_FONT_SIZE  = 25;
const int PongGame::CORNER_FONT_SIZE    = 20;

const std::string PongGame::TITLE         = "pong, kinda";
const std::string PongGame::FONT_FILENAME = "../resources/Inconsolata-Regular.ttf";
const std::string PongGame::BALL_SOUND_FILENAME = "../resources/ball.wav";
const std::string PongGame::MISS_SOUND_FILENAME = "../resources/miss.wav";

const sf::Color PongGame::BACKGROUND(30, 100, 240);
const sf::Color PongGame::MENU_COVER(0, 0, 0, 100);

const int PongGame::POINTS_TO_WIN = 11;

const int PongGame::DIFFICULTY_MIN = -3;
const int PongGame::DIFFICULTY_MAX = 6;

// --------------------

void PongGame::init() {
    // seed the RNG
    std::srand(14315);
    
    // create main window
    window_ = std::unique_ptr<sf::RenderWindow>(
        new sf::RenderWindow(sf::VideoMode(800,600,32),
                             "Ben Zhang's pong - SFML"));
    
    // create AI controller
    ai_ = std::unique_ptr<AutoPlayer>(new AutoPlayer(-1.0f, 2.0f));
    
    // load display font
    if (!font_.loadFromFile(FONT_FILENAME)){
        std::cout << "Is " << FONT_FILENAME 
                  << " in the correct place?" << std::endl;
    }
    
    // load ball sound
    if (!ball_sound_.loadFromFile(BALL_SOUND_FILENAME)){
        std::cout << "Is " << BALL_SOUND_FILENAME
                  << " in the correct place?" << std::endl;
    }
    // load miss sound
    if (!miss_sound_.loadFromFile(MISS_SOUND_FILENAME)){
        std::cout << "Is " << MISS_SOUND_FILENAME
                  << " in the correct place?" << std::endl;
    }
    
    reset();
}

void PongGame::reset() {
    resetEntities();
    resetScore();
    difficulty_ = 0;
    paused_ = true;
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
            
            // Window resized
            else if (evnt.type == sf::Event::Resized) {
                std::cout << "Window resized to " << evnt.size.width 
                          << "x" << evnt.size.height << std::endl;
                window_->setView(sf::View(
                    sf::FloatRect(0, 0, evnt.size.width, evnt.size.height)));
                reset();
                continue;
            }
            
            // Window lost focus / minimized
            else if (evnt.type == sf::Event::LostFocus) {
                std::cout << "Window lost focus." << std::endl;
                // Make sure the game is paused.
                paused_ = true;
            }
            
            else if (evnt.type == sf::Event::KeyPressed) {
                // Escape key exits as well
                if (evnt.key.code == sf::Keyboard::Escape)
                    window_->close();
                // R resets the game
                else if (evnt.key.code == sf::Keyboard::R) {
                    reset();
                    continue;
                }
                // Space starts, pauses, and unpauses
                else if (evnt.key.code == sf::Keyboard::Space) {
                    paused_ = !paused_;
                }
                // Equals / Plus key raises the difficulty
                else if (evnt.key.code == sf::Keyboard::Equal) {
                    if (difficulty_ < DIFFICULTY_MAX){
                       difficulty_++; 
                    }
                }
                // Dash / Minus key lowers the difficulty
                else if (evnt.key.code == sf::Keyboard::Dash) {
                    if (difficulty_ > DIFFICULTY_MIN){
                        difficulty_--;
                    }
                }
                // T toggles whether the center obstacle will be added
                // when objects are reset.
                else if (evnt.key.code == sf::Keyboard::T) {
                    obstacle_ = !obstacle_;
                    if (paused_) {
                        resetEntities();
                    }
                }
                // I toggles AI control of the right paddle ("player 2")
                else if (evnt.key.code == sf::Keyboard::I) {
                    p2_ai_ = !p2_ai_;
                }
                
                // Here are some "cheat codes," of sorts:
                // B nudges the ball slightly, speeding it up
                // and tweaking its trajectory like when it bounces
                else if (evnt.key.code == sf::Keyboard::B) {
                    dynamic_cast<Ball*>(entities_["ball"].get())->boop();
                }
                // pressing 1 emulates the human player scoring a point
                else if (evnt.key.code == sf::Keyboard::Num1) {
                    scorer_ = 1;
                }
                // pressing 2 emulates the AI scoring a point
                else if (evnt.key.code == sf::Keyboard::Num2) {
                    scorer_ = 2;
                }
            }
        }
        
        // check if anyone has scored
        if (scorer_ != 0) {
            switch (scorer_) {
                case 1:
                    p1_score_++;
                    break;
                case 2:
                    p2_score_++;
                    break;
                default:
                    break;
            }
            if (scorer_ == 1) {
                showMessage("< P1 scores.");
            }
            if (scorer_ == 2) {
                showMessage("P2 scores. >");
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
            else if (p2_score_ >= POINTS_TO_WIN) {
                winner = 2;
            }
            if (winner) {
                std::cout << "Player " << winner << " wins!" << std::endl;
                reset();
                if (p2_ai_) {
                    if (winner == 1) {
                        showMessage("You win!");
                    }
                    else {
                        showMessage("The computer wins :(");
                    }
                }
                else {
                    showMessage("Player " + std::to_string(winner) + " wins!");
                }
            }
        }
        
        // read non-event input
        // move p1 paddle
        float p1_move_multiplier = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            p1_move_multiplier += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p1_move_multiplier -= 1;
        applyPaddleInput("p1", p1_move_multiplier);
    
        // move p2 paddle
        float p2_move_multiplier = 0;
        if (p2_ai_) {
            // move AI-controlled paddle
            AutoPlayer::Action ai_action =
                ai_->getAIAction(window_->getSize(), 
                                entities_["ball"]->getPos(), 
                                entities_["ball"]->getVel(), 
                                entities_["p2"]->getPos());
            if (ai_action == AutoPlayer::Action::MOVE_DOWN)
                p2_move_multiplier = 1;
            if (ai_action == AutoPlayer::Action::MOVE_UP)
                p2_move_multiplier = -1;
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                p2_move_multiplier += 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                p2_move_multiplier -= 1;
        }
        
        applyPaddleInput("p2", p2_move_multiplier);
        
        // update entities
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->tick(getTimeScale(), entities_, window_->getSize());
        }
        
        // clear screen and fill with background color
        window_->clear(BACKGROUND);
        
        // draw score counters, difficulty, and notification message
        drawScore();
        
        // draw entities
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            it->second->draw(*window_);
        }
        
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
    // play the woosh effect
    sound_player_.setBuffer(miss_sound_);
    sound_player_.play();
}

void PongGame::ballBounced() {
    // play the bounce effect
    sound_player_.setBuffer(ball_sound_);
    sound_player_.play();
}

void PongGame::centerTextOrigin(sf::Text& text){
    sf::FloatRect text_bounds = text.getLocalBounds();
    text.setOrigin(text_bounds.left + text_bounds.width/2,
                   text_bounds.top + text_bounds.height/2);
}

void PongGame::drawText(sf::Text text, int font_size,
              sf::Vector2f position,
              bool center_origin, bool bold){
    text.setCharacterSize(font_size);
    if (bold) {
        text.setStyle(sf::Text::Bold);
    }
    if (center_origin) {
        centerTextOrigin(text);
    }
    text.setPosition(position);
    window_->draw(text);
}

void PongGame::showMessage(std::string message){
    notification_ = message;
    notification_life_ = NOTIF_LIFETIME;
    notification_opacity_ = 1;
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
    
    // add obstacle if desired
    if (obstacle_){
        entities_["o1"] = std::unique_ptr<Entity>(
            new Paddle(sf::Vector2f(PADDLE_THICKNESS * 2, PADDLE_HEIGHT * 2 / 3), 
                       sf::Vector2f(window_size.x / 2,
                                window_size.y * 2 / 3)));
        entities_["o2"] = std::unique_ptr<Entity>(
            new Paddle(sf::Vector2f(PADDLE_THICKNESS * 2, PADDLE_HEIGHT * 2 / 3), 
                       sf::Vector2f(window_size.x / 2,
                                window_size.y / 3)));
    }
}

void PongGame::resetScore() {
    scorer_ = 0;
    p1_score_ = 0;
    p2_score_ = 0;
    showMessage("<- you are here");
}

void PongGame::drawMenu() {
    // gray out play area with semitransparent covering
    sf::RectangleShape cover;
    cover.setPosition(0,0);
    cover.setSize(sf::Vector2f(window_->getSize().x, window_->getSize().y));
    cover.setFillColor(MENU_COVER);
    window_->draw(cover);
    
    sf::Vector2f half_window(window_->getSize().x / 2, window_->getSize().y / 2);
    
    // draw big title
    sf::Text title_text(TITLE, font_);
    drawText(title_text, TITLE_FONT_SIZE, 
             sf::Vector2f(half_window.x, half_window.y - 120),
             true, true);
    
    // draw control information
    sf::Text tutorial_text("[up]/[down] to move paddle\n"
                           "[R] to reset game\n"
                           "[escape] to quit\n"
                           "[+]/[-] to adjust difficulty\n"
                           "resizing window will reset game\n", font_);
    drawText(tutorial_text, TUTORIAL_FONT_SIZE,
             sf::Vector2f(half_window.x, half_window.y + 150),
             true, false);
}

void PongGame::drawScore() {
    sf::Vector2f half_window(window_->getSize().x / 2,
                             window_->getSize().y / 2);
    
    // draw center line
    sf::RectangleShape divider_line(sf::Vector2f(1, window_->getSize().y));
    divider_line.setPosition(sf::Vector2f(half_window.x, 0));
    window_->draw(divider_line);
    
    // draw left hand player's score
    sf::Text p1_score_text(std::to_string(p1_score_) + "/" +
                           std::to_string(POINTS_TO_WIN), font_);
    drawText(p1_score_text, SCORE_FONT_SIZE,
             sf::Vector2f(half_window.x - half_window.x/2, half_window.y),
             true, true);
    
    // draw right hand player's score
    sf::Text p2_score_text(std::to_string(p2_score_) + "/" +
                           std::to_string(POINTS_TO_WIN), font_);
    drawText(p2_score_text, SCORE_FONT_SIZE,
             sf::Vector2f(half_window.x + half_window.x/2, half_window.y),
             true, true);
    
    // draw central notification message, fade if lifetime expired
    if (notification_life_ > 0){
        notification_life_ -= getTimeScale();
    }
    else{
        notification_opacity_ -= getTimeScale() * NOTIF_FADE_RATE;
        if (notification_opacity_ < 0){
            notification_opacity_ = 0;
        }
    }
    sf::Text notif_text(notification_, font_);
    notif_text.setFillColor(
        sf::Color(255,255,255,(int)(255 * notification_opacity_)));
    drawText(notif_text, CAPTION_FONT_SIZE,
             sf::Vector2f(half_window.x, half_window.y - 50),
             true, false);
    
    // draw bottom-center tip
    sf::Text bottom_tip_text(std::string("press [space] to ") +
                             (paused_ ? "start/unpause" : "pause"), font_);
    drawText(bottom_tip_text, TUTORIAL_FONT_SIZE,
             sf::Vector2f(half_window.x, window_->getSize().y - 50),
             true, false);
    
    // draw difficulty in top left
    std::stringstream difficulty_val_stream;
    difficulty_val_stream << std::fixed 
                          << std::setprecision(2)
                          << getDifficultyTimeMultiplier();
    std::string difficulty_str = describeDifficulty() + " (game speed x" +
                                 difficulty_val_stream.str() + ")";
    sf::Text difficulty_text(difficulty_str, font_);
    drawText(difficulty_text, CORNER_FONT_SIZE, 
             sf::Vector2f(10, 10), 
             false, false);
    
    // draw obstacle / AI indicator in top right
    sf::Text obstacle_text(std::string("obstacle ") +
                           (obstacle_ ? "enabled" : "disabled") + "\n" +
                           "p2 AI " + (p2_ai_ ? "enabled" : "disabled"), font_);
    drawText(obstacle_text, CORNER_FONT_SIZE,
             sf::Vector2f(window_->getSize().x - 
                         (obstacle_text.getLocalBounds().width), 10),
             false, false);
}

float PongGame::getTimeScale() {
    return (1 - (int)paused_) * delta_.asSeconds() * getDifficultyTimeMultiplier();
}

float PongGame::getDifficultyTimeMultiplier() {
    return 1 + (0.25 * difficulty_);
}

std::string PongGame::describeDifficulty() {
    std::string base = "normal";
    if (difficulty_ == 0){
        return base;
    }
    else if (difficulty_ < 0) {
        base = "easy";
        for (int i = difficulty_ + 1; i != 0; i++){
            base = base + "-";
        }
        return base;
    }
    else if (difficulty_ > 0) {
        base = "hard";
        for (int i = difficulty_ - 1; i != 0; i--){
            base = base + "+";
        }
        return base;
    }
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
        paddle->getVel().y + multiplier * getTimeScale() * PADDLE_MOVE_ACCEL,
        -PADDLE_MOVE_SPEED_MAX, PADDLE_MOVE_SPEED_MAX);
    paddle->setVel(sf::Vector2f(0, new_paddle_vy));
}

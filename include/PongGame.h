#ifndef PONGGAME_H
#define PONGGAME_H

#include <SFML/Graphics.hpp>

#include "Paddle.h"
#include "AutoPlayer.h"

class PongGame {
public:
    // Initialize the game.
    void init();
    
    // Reset the game state.
    void reset();
    
    // Run the game's main loop. Returns an exit code when the game stops.
    int gameLoop();
    
private:
    // --------------- Constant declarations
    
    static const float PADDLE_MOVE_SPEED_MAX;
    static const float PADDLE_MOVE_SPEED_BASE;
    static const float PADDLE_MOVE_ACCEL;
    static const float PADDLE_HEIGHT;
    static const float PADDLE_THICKNESS;
    static const float PADDLE_BACK_DISTANCE;
    static const float BALL_START_SPEED;
    static const float BALL_START_ANGLE_RANGE;
    static const sf::Color BACKGROUND;
    
    // ---------
    
    // --------------- Fields and members for game data
    
    // main game window
    std::unique_ptr<sf::RenderWindow> window_;
    
    // clock for delta time
    sf::Clock frame_timer_;
    // time between the game launching and the previous frame
    sf::Time last_frame_;
    // time between the last and current frame
    sf::Time delta_;
    
    // flag indicating someone has scored;
    // nonzero during loop iterations when someone has scored
    int scorer_ = 0;
    
    // cumulative score trackers
    int p1_score_ = 0;
    int p2_score_ = 0;
    
    // create AI controller
    std::unique_ptr<AutoPlayer> ai_;
    
    // set up map for entities
    ENTITY_MAP entities_;
    
    // ---------
    
    // --------------- Private functions
    
    void resetEntities();
    void resetScore();
    
    void applyPaddleInput(std::string paddle_id, float multiplier);
    
    // ---------
};

#endif // PONGGAME_H

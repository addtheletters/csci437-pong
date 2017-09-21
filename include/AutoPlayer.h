#ifndef AUTO_H
#define AUTO_H

#include <SFML/Graphics.hpp>

// This class describes AI-controlled paddle behavior.
class AutoPlayer {
public:
    AutoPlayer(float away, float laziness) {
        away_ = away;
        laziness_ = laziness;
    }
    
    enum Action {
        NONE = 0,
        MOVE_UP    = 1,
        MOVE_DOWN  = 2,
    };
    
    // Get the action this AI actor should take, based on speed and position
    // of the ball, the position of the paddle the AI controls, the screen
    // area.
    Action getAIAction(sf::Vector2u window_size, 
                       sf::Vector2f ball_pos,
                       sf::Vector2f ball_speed,
                       sf::Vector2f paddle_pos);
    
private:
    // A positive or negative value whose sign designates the 'away' direction.
    float away_;
    
    // Allowed vertical distance from the ball within which the AI takes no action.
    float laziness_;
};

#endif // AUTO_H

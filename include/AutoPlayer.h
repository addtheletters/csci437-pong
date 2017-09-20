#ifndef AUTO_H
#define AUTO_H

#include <SFML/Graphics.hpp>

class AutoPlayer {
public:
    enum Action {
        NONE = 0,
        MOVE_UP    = 1,
        MOVE_DOWN  = 2,
    };
    
    // Get the action this AI actor should take, based on speed and position
    // of the ball, the position of the paddle the AI controls, the screen
    // area, and a positive or negative value designating the 'away' direction.
    // Since the paddle has width, the AI is allowed some laziness in its distance
    // from the ball.
    Action getAIAction(sf::Vector2u window_size, 
                        sf::Vector2f ball_pos,
                        sf::Vector2f ball_speed,
                        sf::Vector2f paddle_pos,
                        float away,
                        float laziness
                      );
};

#endif // AUTO_H

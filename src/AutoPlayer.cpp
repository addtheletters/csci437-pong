#include "AutoPlayer.h"

#include <cmath>

AutoPlayer::Action AutoPlayer::getAIAction(sf::Vector2u window_size, 
                        sf::Vector2f ball_pos,
                        sf::Vector2f ball_vel,
                        sf::Vector2f paddle_pos,
                        float away,
                        float laziness){
    float target_x = ball_pos.x;
    // if the ball is moving away, move to a more central reset position
    if ((away > 0) == (ball_vel.y > 0)){
        target_x = (ball_pos.x + (window_size.x / 2)) / 2.0f;
    }
    float diff = target_x - paddle_pos.x;
    float diff_abs = abs(diff);
    if (diff_abs < laziness) {
        return AutoPlayer::Action::NONE;
    }
    if (diff > 0) {
        return AutoPlayer::Action::MOVE_RIGHT;
    }
    else{
        return AutoPlayer::Action::MOVE_LEFT;
    }
}

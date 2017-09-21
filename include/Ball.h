#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "ScoreInterface.h"

// This class represents the ball.
// It is responsible for its own movement, and thus handles the ball's 
// bouncing behavior and resolution of collisions with paddles.
// Its entity position is the center of its circular shape.
class Ball : public Entity {
public:
    // increase speedup for more ball acceleration upon bounces
    const float BOOP_SPEEDUP = 15.0f; // 10
    // decrease stability for more random angle deviation
    const float BOOP_STABILITY = 7.0f;
    // fraction of a paddle's velocity that gets added to the ball's upon hit
    const float SPEED_INHERITANCE = 0.15f; // 0.2
    
    Ball(float radius, sf::Vector2f center, ScoreInterface* score_notify);
    
    void draw(sf::RenderWindow&) override;
    void tick(float delta, ENTITY_MAP& others,
              sf::Vector2u window_size) override;
    
    // Returns the ball's radius.
    float getRad();
    
    // Set the ball's radius.
    void setRad(float rad);
    
    // Add some speed and make a random angle tweak; to be called on bounces
    void boop();
    
private:
    // The ball's radius
    float rad_;
    
    // Interface to which the ball reports when it reaches certain states,
    // such as for adding score or playing audio
    ScoreInterface* score_notify_;
};

#endif // BALL_H

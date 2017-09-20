#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Ball : public Entity {
public:
    // increase speedup for more ball acceleration upon bounces
    const float BOOP_SPEEDUP = 1.0f;
    // decrease stability for more random angle deviation
    const float BOOP_STABILITY = 1.0f;
    
    // fraction of a paddle's velocity that gets added to the ball's upon hit
    const float SPEED_INHERITANCE = 0.2f;
    
    Ball(float radius);
    
    void draw(sf::RenderWindow&) override;
    void tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size) override;
    
    float getRad();
    void setRad(float new_radius);
    
    // Add some speed and make a random angle tweak; to be called on bounces
    void boop();
    
private:
    float rad;
};

#endif // BALL_H

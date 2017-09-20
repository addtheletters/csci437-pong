#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Ball : public Entity {
public:
    // increase speedup for more ball acceleration upon bounces
    const float BOOP_SPEEDUP = 10.0f;
    // decrease stability for more random angle deviation
    const float BOOP_STABILITY = 5.0f;
    
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

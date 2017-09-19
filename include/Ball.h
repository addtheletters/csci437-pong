#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Ball : public Entity {
public:
    Ball(float radius);
    
    void draw(sf::RenderWindow&) override;
    void tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size) override;
    
    float getRad();
    void setRad(float new_radius);
    
private:
    float rad;
};

#endif // BALL_H

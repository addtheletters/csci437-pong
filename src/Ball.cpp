#include <iostream>
#include "Ball.h"

Ball::Ball(float radius){
    rad = radius;
}

float Ball::getRad(){
    return rad;
}

void Ball::setRad(float new_radius){
    rad = new_radius;
}

void Ball::draw(sf::RenderWindow& window){
    //std::cout << "Bogus draw of ball with rad " << getRad() << std::endl;
    //std::cout << "Vel " << getVel().x << "," << getVel().y << "... rad " << getRad() << std::endl;
    
    sf::CircleShape circ;
    circ.setRadius(rad);
    circ.setOrigin(rad, rad);
    circ.setPosition(pos);
    
    window.draw(circ);
}

void Ball::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    // move according to velocity
    Entity::tick(delta, others, window_size);
    
    // handle collisions with borders
    // right wall
    if (pos.x + rad > window_size.x){
        pos.x = window_size.x - rad;
        vel.x = -vel.x;
    }
    // left wall
    if (pos.x - rad < 0){
        pos.x = rad;
        vel.x = -vel.x;
    }
    // bottom wall
    if (pos.y + rad > window_size.y){
        pos.y = window_size.y - rad;
        vel.y = -vel.y;
    }
    // top wall
    if (pos.y - rad < 0){
        pos.y = rad;
        vel.y = -vel.y;
    }
    
    //std::cout << "Ball was tick'd with time " << delta.asMicroseconds() << std::endl;
}

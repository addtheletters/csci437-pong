#include "Ball.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

#include "Paddle.h"

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
    sf::CircleShape circ;
    circ.setRadius(rad);
    circ.setOrigin(rad, rad);
    circ.setPosition(pos);
    window.draw(circ);
}

void Ball::boop(){
    int boop_angle_index = (std::rand() % 360) - 180;
    adjustVel(BOOP_SPEEDUP * .5, (1.0f * boop_angle_index) / (360 * BOOP_STABILITY) );
    // trend to go towards the edges
    if (vel.x < 0){
        vel.x -= BOOP_SPEEDUP * .5;
    }
    else{
        vel.x += BOOP_SPEEDUP * .5;
    }
    
    std::cout << "Boop! - " << boop_angle_index << std::endl;
}

void Ball::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    // move according to velocity
    Entity::tick(delta, others, window_size);
    
    // handle collisions with borders
    // right wall
    if (pos.x + rad > window_size.x){
        pos.x = window_size.x - rad;
        vel.x = -vel.x;
        //boop();
    }
    // left wall
    if (pos.x - rad < 0){
        pos.x = rad;
        vel.x = -vel.x;
        //boop();
    }
    // bottom wall
    if (pos.y + rad > window_size.y){
        pos.y = window_size.y - rad;
        vel.y = -vel.y;
        //boop();
    }
    // top wall
    if (pos.y - rad < 0){
        pos.y = rad;
        vel.y = -vel.y;
        //boop();
    }
    
    // handle collisions with other entities
    for (auto it = others.begin(); it != others.end(); it++){
        if (it->first == "ball")
            continue;
        if (it->first == "p1" || it->first == "p2"){
            // do bouncy bounce on paddles
            Paddle* paddle = dynamic_cast<Paddle*>(it->second.get());
            int hits = paddle->checkBallCollision(getPos(), getRad());
            
            if (hits & Paddle::ContactStatus::TOP){
                //std::cout << "Hit top of paddle " << it->first << std::endl;
                if (vel.y > 0){
                    vel.y = -vel.y;
                    //boop();
                }
                pos.y = paddle->getPos().y - (paddle->getSize().y / 2) - getRad();
            }
            if (hits & Paddle::ContactStatus::LEFT){
                std::cout << "Hit left of paddle " << it->first 
                          << " giving speed " << SPEED_INHERITANCE * paddle->getVel().y
                          << std::endl;
                if (vel.x > 0){
                    vel.x = -vel.x;
                    boop();
                }
                pos.x = paddle->getPos().x - (paddle->getSize().x / 2) - getRad();
                // add some paddle velocity to the ball
                vel.y = vel.y * (1.0-SPEED_INHERITANCE) + (SPEED_INHERITANCE * paddle->getVel().y);
            }
            if (hits & Paddle::ContactStatus::BOTTOM){
                //std::cout << "Hit bottom of paddle " << it->first << std::endl;
                if (vel.y < 0){
                    vel.y = -vel.y;
                    //boop();
                }
                pos.y = paddle->getPos().y + (paddle->getSize().y / 2) + getRad();
            }
            if (hits & Paddle::ContactStatus::RIGHT){
                std::cout << "Hit right of paddle " << it->first
                          << " giving speed " << SPEED_INHERITANCE * paddle->getVel().y
                          << std::endl;
                if (vel.x < 0){
                    vel.x = -vel.x;
                    boop();
                }
                pos.x = paddle->getPos().x + (paddle->getSize().x / 2) + getRad();
                // add some paddle velocity to the ball
                vel.y = vel.y * (1.0-SPEED_INHERITANCE) + (SPEED_INHERITANCE * paddle->getVel().y);
            }
        }
    }
}

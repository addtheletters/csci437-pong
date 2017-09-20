#include "Ball.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <memory>

#include "Paddle.h"

Ball::Ball(float radius, sf::Vector2f center){
    rad_ = radius;
    pos_ = center;
}

float Ball::getRad(){
    return rad_;
}

void Ball::setRad(float rad){
    rad_ = rad;
}

void Ball::draw(sf::RenderWindow& window){
    sf::CircleShape circ;
    circ.setRadius(rad_);
    circ.setOrigin(rad_, rad_);
    circ.setPosition(pos_);
    window.draw(circ);
}

void Ball::boop(){
    int boop_angle_index = (std::rand() % 360) - 180;
    adjustVel(BOOP_SPEEDUP * .5, (1.0f * boop_angle_index) / (360 * BOOP_STABILITY) );
    // trend to go towards the edges
    if (vel_.x < 0){
        vel_.x -= BOOP_SPEEDUP * .5;
    }
    else{
        vel_.x += BOOP_SPEEDUP * .5;
    }
    
    std::cout << "Boop! - " << boop_angle_index << std::endl;
}

void Ball::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    // move according to velocity
    Entity::tick(delta, others, window_size);
    
    // handle collisions with borders
    // right wall
    if (pos_.x + rad_ > window_size.x){
        pos_.x = window_size.x - rad_;
        vel_.x = -vel_.x;
        //boop();
    }
    // left wall
    if (pos_.x - rad_ < 0){
        pos_.x = rad_;
        vel_.x = -vel_.x;
        //boop();
    }
    // bottom wall
    if (pos_.y + rad_ > window_size.y){
        pos_.y = window_size.y - rad_;
        vel_.y = -vel_.y;
        boop();
    }
    // top wall
    if (pos_.y - rad_ < 0){
        pos_.y = rad_;
        vel_.y = -vel_.y;
        boop();
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
                if (vel_.y > 0){
                    vel_.y = -vel_.y;
                    //boop();
                }
                pos_.y = paddle->getPos().y - (paddle->getSize().y / 2) - getRad();
            }
            if (hits & Paddle::ContactStatus::LEFT){
                std::cout << "Hit left of paddle " << it->first 
                          << " giving speed " << SPEED_INHERITANCE * paddle->getVel().y
                          << std::endl;
                if (vel_.x > 0){
                    vel_.x = -vel_.x;
                    boop();
                }
                pos_.x = paddle->getPos().x - (paddle->getSize().x / 2) - getRad();
                // add some paddle velocity to the ball
                vel_.y += (SPEED_INHERITANCE * paddle->getVel().y);
            }
            if (hits & Paddle::ContactStatus::BOTTOM){
                //std::cout << "Hit bottom of paddle " << it->first << std::endl;
                if (vel_.y < 0){
                    vel_.y = -vel_.y;
                    //boop();
                }
                pos_.y = paddle->getPos().y + (paddle->getSize().y / 2) + getRad();
            }
            if (hits & Paddle::ContactStatus::RIGHT){
                std::cout << "Hit right of paddle " << it->first
                          << " giving speed " << SPEED_INHERITANCE * paddle->getVel().y
                          << std::endl;
                if (vel_.x < 0){
                    vel_.x = -vel_.x;
                    boop();
                }
                pos_.x = paddle->getPos().x + (paddle->getSize().x / 2) + getRad();
                // add some paddle velocity to the ball
                vel_.y += (SPEED_INHERITANCE * paddle->getVel().y);
            }
        }
    }
}

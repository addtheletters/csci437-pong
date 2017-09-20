#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Paddle : public Entity {
public:
    enum ContactStatus {
        // binary flags so we know when we have contact with any of the sides
        NONE   = 0,
        LEFT   = 1,
        RIGHT  = 2,
        TOP    = 4,
        BOTTOM = 8,
    };
    
    Paddle(sf::Vector2f size, sf::Vector2f center);
    
    void draw(sf::RenderWindow&) override;
    void tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size) override;
    
    sf::Vector2f getSize();
    
    // Attempt to move horizontally. Return false if the paddle is stopped by a wall,
    // true if the paddle moves the entirety of the amount.
    bool tryMoveHorizontal(float amount, sf::Vector2u window_size);
    
    // Check if given ball is touching this paddle. Return a ContactStatus.
    int checkBallCollision(sf::Vector2f center, float radius);
    
private:
    sf::Vector2f size;
};

#endif // PADDLE_H

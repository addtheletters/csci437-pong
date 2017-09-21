#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

#include "Entity.h"

// This class represents rectangular objects with which the ball may collide.
// It handles detection of collisions with circular objects, but not resolution
// of those collisions.
// It can move vertically within the window's constraints.
// Its rectangle is centered on its entity position.
class Paddle : public Entity {
public:
    // Enumerates possible sides of this paddle that may contact something 
    // that is colliding with it.
    enum ContactStatus {
        // The values are usable as binary flags in case we decide 
        // is in contact with many sides at once.
        NONE   = 0,
        LEFT   = 1,
        RIGHT  = 2,
        TOP    = 4,
        BOTTOM = 8,
    };
    
    Paddle(sf::Vector2f size, sf::Vector2f center);
    
    void draw(sf::RenderWindow&) override;
    void tick(float delta, ENTITY_MAP& others,
                sf::Vector2u window_size) override;
    
    // Returns the dimensions of this object's rectangular shape.
    sf::Vector2f getSize();
    
    // Attempt to move vertically. Return false if the paddle is stopped by
    // a window border, true if the paddle moves the entirety of the amount.
    bool tryMoveVertical(float amount, sf::Vector2u window_size);
    
    // Check if a circle is intersecting this paddle. Return a ContactStatus,
    // or potentially several as binary flags combined into an int. 
    int checkBallCollision(sf::Vector2f center, float radius);
    
private:
    sf::Vector2f size_;
    
    ContactStatus closestCardinal(sf::Vector2f);
};

#endif // PADDLE_H

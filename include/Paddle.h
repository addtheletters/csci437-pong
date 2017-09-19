#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Paddle : public Entity {
public:
    Paddle(sf::Vector2f size, sf::Vector2f center);
    
    void draw(sf::RenderWindow&) override;
    void tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size) override;
    
    sf::Vector2f getSize();
    
private:
    sf::Vector2f size;
};

#endif // PADDLE_H

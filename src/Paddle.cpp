#include <iostream>
#include "Paddle.h"

Paddle::Paddle(sf::Vector2f size, sf::Vector2f center){
    this->size = size;
    pos = center;
}

void Paddle::draw(sf::RenderWindow& window){
    sf::RectangleShape box;
    box.setSize(size);
    box.setFillColor(sf::Color::Red);
    box.setOrigin(size.x / 2, size.y / 2);
    box.setPosition(getPos());
    window.draw(box);
    //std::cout << "Bogus draw of paddle " << std::endl;
}

void Paddle::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    Entity::tick(delta, others, window_size);
    //std::cout << "Paddle was tick'd with time " << delta.asMicroseconds() << std::endl;
}

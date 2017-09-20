#include "Paddle.h"

#include <iostream>
#include <cmath>

#include "Ball.h"
#include "VectorUtility.h"


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
}

void Paddle::tick(sf::Time delta, ENTITY_MAP& others,
                  sf::Vector2u window_size){
    Entity::tick(delta, others, window_size);
    float half_height = size.y / 2;
    if (pos.y - half_height < 0) {
        pos.y = half_height;
        // thud, paddle hits top border
    }
    if (pos.y + half_height > window_size.y) {
        pos.y = window_size.y - half_height;
        // thud, paddle hits bottom border
    }
}

sf::Vector2f Paddle::getSize(){
    return size;
}

// Determines the closest axial / cardinal direction to a direction
// vector, and returns a corresponding ContactStatus for that rectangle side.
Paddle::ContactStatus closestCardinal(sf::Vector2f dir){
    sf::Vector2f cardinals[] = {
        sf::Vector2f(0, 1),
        sf::Vector2f(1, 0),
        sf::Vector2f(0, -1),
        sf::Vector2f(-1, 0)
    };
    float max = 0;
    int best = 0;
    sf::Vector2f normalized = VecUtil::normalize(dir);
    
    std::cout << "norm x " << normalized.x << std::endl;
    std::cout << "norm y " << normalized.y << std::endl;
    std::cout << "confirmed len " << VecUtil::dotProd(normalized, normalized) << std::endl;
    
    for (int i = 0; i < 4; i++){
        float dp = VecUtil::dotProd(normalized, cardinals[i]);
        if (dp > max){
            max = dp;
            best = i;
        }
    }
    switch (best){
        case 0:
            return Paddle::ContactStatus::TOP;
        case 1:
            return Paddle::ContactStatus::LEFT;
        case 2:
            return Paddle::ContactStatus::BOTTOM;
        case 3:
            return Paddle::ContactStatus::RIGHT;
        default:
            return Paddle::ContactStatus::NONE;
    }
}

int Paddle::checkBallCollision(sf::Vector2f center, float radius){
    sf::Vector2f half_size = size * 0.5f;
    sf::Vector2f diff = center - pos;
    sf::Vector2f closest = pos + VecUtil::clampVec2(diff, -half_size, half_size);
    diff = closest - center;
    int contacts = ContactStatus::NONE;
    if (VecUtil::dotProd(diff, diff) <= radius * radius) {
        return closestCardinal(diff);
    }
    return contacts;
}

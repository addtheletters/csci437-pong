#include "Paddle.h"

#include <iostream>
#include <cmath>

#include "Ball.h"
#include "VectorUtility.h"


Paddle::Paddle(sf::Vector2f size, sf::Vector2f center){
    size_ = size;
    pos_ = center;
}

void Paddle::draw(sf::RenderWindow& window){
    sf::RectangleShape box;
    box.setSize(size_);
    box.setFillColor(sf::Color::Red);
    box.setOrigin(size_.x / 2, size_.y / 2);
    box.setPosition(getPos());
    window.draw(box);
}

void Paddle::tick(float delta, ENTITY_MAP& others,
                  sf::Vector2u window_size){
    Entity::tick(delta, others, window_size);
    float half_height = size_.y / 2;
    if (pos_.y - half_height < 0) {
        pos_.y = half_height;
        // thud, paddle hits top border
    }
    if (pos_.y + half_height > window_size.y) {
        pos_.y = window_size.y - half_height;
        // thud, paddle hits bottom border
    }
}

sf::Vector2f Paddle::getSize(){
    return size_;
}

// Determines the closest axial / cardinal direction to a direction
// vector, and returns a corresponding ContactStatus for that rectangle side.
Paddle::ContactStatus Paddle::closestCardinal(sf::Vector2f dir){
    sf::Vector2f cardinals[] = {
        sf::Vector2f(0, 1),
        sf::Vector2f(1, 0),
        sf::Vector2f(0, -1),
        sf::Vector2f(-1, 0)
    };
    float max = 0;
    int best = 0;
    sf::Vector2f normalized = vecutil::normalize(dir);
    for (int i = 0; i < 4; i++){
        float dp = vecutil::dotProd(normalized, cardinals[i]);
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
    sf::Vector2f half_size = size_ * 0.5f;
    sf::Vector2f diff = center - pos_;
    sf::Vector2f closest = pos_ + vecutil::clampVec2(diff, -half_size, half_size);
    diff = closest - center;
    int contacts = ContactStatus::NONE;
    if (vecutil::dotProd(diff, diff) <= radius * radius) {
        return closestCardinal(diff);
    }
    return contacts;
}

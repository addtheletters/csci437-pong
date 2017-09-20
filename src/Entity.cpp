#include "Entity.h"

#include <iostream>

#include "VectorUtility.h"

Entity::Entity(){
    pos_ = sf::Vector2f(0, 0);
    vel_ = sf::Vector2f(0, 0);
}

Entity::~Entity(){
    std::cout << "~Entity" << std::endl;
}

void Entity::draw(sf::RenderWindow& window) {
    return;
}

void Entity::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    move(getVel() * delta.asSeconds());
    return;
}

sf::Vector2f Entity::getPos(){
    return pos_;
}

void Entity::setPos(sf::Vector2f pos){
    pos_ = pos;
}

void Entity::move(sf::Vector2f displacement){
    pos_ = pos_ + displacement;
}

sf::Vector2f Entity::getVel(){
    return vel_;
}

void Entity::setVel(sf::Vector2f vel){
    vel_ = vel;
}

void Entity::adjustVel(float speed_diff, float angle_diff){
    float mag = sqrt(vel_.x * vel_.x + vel_.y * vel_.y) + speed_diff;
    float ang = atan2(vel_.y, vel_.x) + angle_diff;
    setVel(vecutil::fromPolar(mag, ang));   
}

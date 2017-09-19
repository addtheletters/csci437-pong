#include "Entity.h"

Entity::Entity(){
    pos = sf::Vector2f(0, 0);
    vel = sf::Vector2f(0, 0);
}

void Entity::draw(sf::RenderWindow& window) {
    return;
}

void Entity::tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size){
    move(getVel() * delta.asSeconds());
    return;
}

sf::Vector2f Entity::getPos(){
    return pos;
}

void Entity::setPos(sf::Vector2f new_position){
    pos = new_position;
}

void Entity::move(sf::Vector2f displacement){
    pos = pos + displacement;
}

sf::Vector2f Entity::getVel(){
    return vel;
}

void Entity::setVel(sf::Vector2f new_velocity){
    vel = new_velocity;
}

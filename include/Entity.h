#ifndef ENTITY_MAP
#define ENTITY_MAP std::map<std::string, std::unique_ptr<Entity>>
#endif

#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

class Entity {
    
friend class Paddle;
friend class Ball;
    
public:
    
    // Draw the entity in SFML graphics
    virtual void draw(sf::RenderWindow&);
    
    // Update the entity's game state
    virtual void tick(sf::Time delta, ENTITY_MAP& others, sf::Vector2u window_size);
    
    // Return the current position
    sf::Vector2f getPos();
    
    // Place this entity at a new position
    void setPos(sf::Vector2f new_position);
    
    // Displace this entity relative to its current position
    void move(sf::Vector2f displacement);
    
    // Get the current velocity
    sf::Vector2f getVel();
    
    // Set the velocity vector directly
    void setVel(sf::Vector2f new_velocity);
    
    // ... set the velocity angle
    // ... set the speed. Choose a random angle if speed was 0 and changes to non-zero.
    
private:
    Entity();
    
    sf::Vector2f pos;
    sf::Vector2f vel;
};

#endif // ENTITY_H

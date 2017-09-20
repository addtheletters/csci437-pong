#ifndef ENTITY_MAP
#define ENTITY_MAP std::map<std::string, std::unique_ptr<Entity>>
#endif

#ifndef ENTITY_H
#define ENTITY_H

#include <cmath>
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

// This class represents all displayed objects present in gameplay,
// capable of moving, and interacting with other entities.
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
    
    // Set the velocity using a speed and direction (angle in radians)
    void setVel(float speed, float angle){
        setVel(sf::Vector2f(speed * cos(angle), speed * sin(angle)));
    };
    
    // Get the speed (magnitude of velocity vector).
    float getSpeed(){
        return sqrt(vel.x * vel.x + vel.y * vel.y);
    };
    
    // Get the angle of the velocity vector.
    float getAngle(){
        return atan2(vel.y, vel.x);
    };
    
    // Change the velocity's speed and angle
    void adjustVel(float speed_diff, float angle_diff);
    
private:
    Entity();
    
    sf::Vector2f pos;
    sf::Vector2f vel;
};

#endif // ENTITY_H

#ifndef ENTITY_MAP
#define ENTITY_MAP std::map<std::string, std::unique_ptr<Entity>>
#endif

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>
#include <memory>
#include <string>

// This class represents all displayed gameplay objects which are
// capable of moving, and interacting with other entities.
class Entity {

  friend class Paddle;
  friend class Ball;

public:
  Entity();

  // Draw the entity in SFML graphics
  virtual void draw(sf::RenderWindow &);

  // Update the entity's game state
  virtual void tick(float delta, ENTITY_MAP &others, sf::Vector2u window_size);

  // Return the current position
  sf::Vector2f getPos();

  // Place this entity at a new position
  void setPos(sf::Vector2f pos);

  // Displace this entity relative to its current position
  void move(sf::Vector2f displacement);

  // Get the current velocity
  sf::Vector2f getVel();

  // Set the current velocity
  void setVel(sf::Vector2f vel);

  // Change the velocity's speed and angle
  void adjustVel(float speed_diff, float angle_diff);

private:
  sf::Vector2f pos_;
  sf::Vector2f vel_;
};

#endif // ENTITY_H

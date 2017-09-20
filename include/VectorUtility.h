#ifndef VECUTIL_H
#define VECUTIL_H

#include <SFML/Graphics.hpp>

namespace VecUtil{

// Enforces lower and upper bounds on val.
float clamp(float val, float cmin, float cmax);

// Enforces bounds (defined by vectors min and max)
// on elements of vector val.
sf::Vector2f clampVec2(sf::Vector2f val, sf::Vector2f min, sf::Vector2f max);

// Returns a dot product of two vectors.
float dotProd(sf::Vector2f, sf::Vector2f);

// Returns the magnitude of vec.
float length(sf::Vector2f);

// Returns a normalized (1-length) vector in the direction of the provided,
// or a zero-vector if provided with a zero-vector.
sf::Vector2f normalize(sf::Vector2f);

}

#endif

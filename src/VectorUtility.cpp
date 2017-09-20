#include "VectorUtility.h"

#include <cmath>

// This file holds implementations of VecUtil utility functions.

float VecUtil::clamp(float val, float cmin, float cmax){
    return std::max(cmin, std::min(cmax, val));
}

float VecUtil::dotProd(sf::Vector2f a, sf::Vector2f b){
    return a.x * b.x + a.y * b.y;
}

float VecUtil::length(sf::Vector2f vec){
    return sqrt(dotProd(vec, vec));
}

sf::Vector2f VecUtil::normalize(sf::Vector2f vec){
    if (vec.x == 0 && vec.y == 0){
        return sf::Vector2f(0,0);
    }
    return vec / VecUtil::length(vec);
}

sf::Vector2f VecUtil::clampVec2(sf::Vector2f val, sf::Vector2f min, sf::Vector2f max){
    return sf::Vector2f(clamp(val.x, min.x, max.x), clamp(val.y, min.y, max.y));
}

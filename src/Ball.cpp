#include "Ball.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "Paddle.h"

Ball::Ball(float radius, sf::Vector2f center, ScoreInterface *score_notify) {
  rad_ = radius;
  pos_ = center;
  score_notify_ = score_notify;
}

float Ball::getRad() { return rad_; }

void Ball::setRad(float rad) { rad_ = rad; }

void Ball::draw(sf::RenderWindow &window) {
  sf::CircleShape circ;
  circ.setRadius(rad_);
  circ.setOrigin(rad_, rad_);
  circ.setPosition(pos_);
  circ.setFillColor(sf::Color(255, 165, 0));
  window.draw(circ);
}

void Ball::boop() {
  score_notify_->ballBounced();

  int boop_angle_index = (std::rand() % 360) - 180;
  adjustVel(BOOP_SPEEDUP, (1.0f * boop_angle_index) / (360 * BOOP_STABILITY));
  // trend to go towards the edges
  if (vel_.x < 0) {
    vel_.x -= BOOP_SPEEDUP;
  } else {
    vel_.x += BOOP_SPEEDUP;
  }
}

void Ball::tick(float delta, ENTITY_MAP &others, sf::Vector2u window_size) {
  // move according to velocity
  Entity::tick(delta, others, window_size);

  // handle collisions with borders
  // right wall, allow ball to completely vanish before counting score
  if (pos_.x - rad_ > window_size.x) {
    score_notify_->playerScored(1);
  }
  // left wall, allow ball to completely vanish before counting score
  if (pos_.x + rad_ < 0) {
    score_notify_->playerScored(2);
  }
  // bottom wall
  if (pos_.y + rad_ > window_size.y) {
    pos_.y = window_size.y - rad_;
    vel_.y = -vel_.y;
    boop();
  }
  // top wall
  if (pos_.y - rad_ < 0) {
    pos_.y = rad_;
    vel_.y = -vel_.y;
    boop();
  }

  // handle collisions with other entities
  for (auto it = others.begin(); it != others.end(); it++) {
    if (it->first == "ball")
      continue;
    if (it->first == "p1" || it->first == "p2" || it->first == "o1" ||
        it->first == "o2") {
      // do bouncy bounce on paddles and obstacles
      Paddle *paddle = dynamic_cast<Paddle *>(it->second.get());
      int hits = paddle->checkBallCollision(getPos(), getRad());

      if (hits & Paddle::ContactStatus::TOP) {
        if (vel_.y > 0) {
          vel_.y = -vel_.y;
          boop();
        }
        pos_.y = paddle->getPos().y - (paddle->getSize().y / 2) - getRad();
      }
      if (hits & Paddle::ContactStatus::LEFT) {
        // std::cout << "Hit left of paddle " << it->first
        //          << " giving speed " << SPEED_INHERITANCE *
        //          paddle->getVel().y
        //          << std::endl;
        if (vel_.x > 0) {
          vel_.x = -vel_.x;
          boop();
        }
        pos_.x = paddle->getPos().x - (paddle->getSize().x / 2) - getRad();
        // add some paddle velocity to the ball
        vel_.y += SPEED_INHERITANCE * paddle->getVel().y;
      }
      if (hits & Paddle::ContactStatus::BOTTOM) {
        if (vel_.y < 0) {
          vel_.y = -vel_.y;
          boop();
        }
        pos_.y = paddle->getPos().y + (paddle->getSize().y / 2) + getRad();
      }
      if (hits & Paddle::ContactStatus::RIGHT) {
        // std::cout << "Hit right of paddle " << it->first
        //          << " giving speed " << SPEED_INHERITANCE *
        //          paddle->getVel().y
        //         << std::endl;
        if (vel_.x < 0) {
          vel_.x = -vel_.x;
          boop();
        }
        pos_.x = paddle->getPos().x + (paddle->getSize().x / 2) + getRad();
        // add some paddle velocity to the ball
        vel_.y += SPEED_INHERITANCE * paddle->getVel().y;
      }
    }
  }
}

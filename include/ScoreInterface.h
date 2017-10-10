#ifndef I_SCORE_H
#define I_SCORE_H

// Interface implemented by the main game class through which game objects
// report when scoring occurs, avoiding circular dependencies.
class ScoreInterface {
public:
  // Notify the game that a player has scored.
  virtual void playerScored(int scorer) = 0;

  // Notify the game that the ball has bounced.
  virtual void ballBounced() = 0;
};

#endif // I_SCORE_H

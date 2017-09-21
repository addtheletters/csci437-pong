#ifndef PONGGAME_H
#define PONGGAME_H

#include <SFML/Graphics.hpp>

#include "ScoreInterface.h"
#include "Paddle.h"
#include "AutoPlayer.h"

class PongGame : ScoreInterface {
public:
    // Initialize the game.
    void init();
    
    // Reset the game state.
    void reset();
    
    // Run the game's main loop. Returns an exit code when the game stops.
    int gameLoop();
    
    // Implementing the ScoreInterface
    void playerScored(int scorer) override;
    
private:
    // --------------- Constant declarations
    
    static const float PADDLE_MOVE_SPEED_MAX;
    static const float PADDLE_MOVE_SPEED_BASE;
    static const float PADDLE_MOVE_ACCEL;
    static const float PADDLE_HEIGHT;
    static const float PADDLE_THICKNESS;
    static const float PADDLE_BACK_DISTANCE;
    static const float BALL_START_SPEED;
    static const float BALL_START_ANGLE_RANGE;
    static const int SCORE_FONT_SIZE;
    static const int TITLE_FONT_SIZE;
    static const int CAPTION_FONT_SIZE;
    static const int POINTS_TO_WIN;
    static const std::string TITLE;
    static const std::string FONT_FILENAME;
    static const sf::Color BACKGROUND;
    
    // ----------------------------------
    
    // --------------- Fields and members for game data
    
    // main game window
    std::unique_ptr<sf::RenderWindow> window_;
    
    // display font
    sf::Font font_;
    
    // "how-to-play" / paused text object
    // "(re)start" controls
    // this might not need on-the-fly modding either/
    sf::Text tutorial;
    // notification text object
    sf::Text notification;
    
    // clock for delta time
    sf::Clock frame_timer_;
    // time between the game launching and the previous frame
    sf::Time last_frame_;
    // time between the last and current frame
    sf::Time delta_;
    
    // multiplier to be applied to time delta to allow pausing and slowdowns
    float time_scale_ = 1.0f;
    
    // is the game paused? are we in the menu?
    bool paused_ = true;
    
    // flag indicating someone has scored;
    // nonzero during loop iterations when someone has scored
    int scorer_ = 0;
    
    // cumulative score trackers
    int p1_score_ = 0;
    int p2_score_ = 0;
    
    // create AI controller
    std::unique_ptr<AutoPlayer> ai_;
    
    // set up map for entities
    ENTITY_MAP entities_;
    
    // -------------------------------
    
    // --------------- Private functions
    
    // Sets the origin of a SFML text object to the center of its
    // bounding rectangle, so later setting the text position
    // will place the center of the text at that position.
    void centerTextOrigin(sf::Text& text);
    
    // Display a message to the middle of the play area, which 
    // fades over time while the game is unpaused.
    void showMessage(std::string message);
    
    void resetEntities();
    void resetScore();
    
    void drawMenu();
    void drawScore();
    
    void applyPaddleInput(std::string paddle_id, float multiplier);
    
    // ------------------------
};

#endif // PONGGAME_H

#ifndef PONGGAME_H
#define PONGGAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "ScoreInterface.h"
#include "Paddle.h"
#include "AutoPlayer.h"

// This class represents the entirety of the running game.
// It holds all relevant values and objects.
class PongGame : ScoreInterface {
public:
    // Initialize the game.
    void init();
    
    // Reset the game state.
    void reset();
    
    // Run the game's main loop. Returns an exit code when the game stops.
    int gameLoop();
    
    // Implementing ScoreInterface...
    // Set a flag to tell the game that a point has been scored;
    // Play the miss / woosh sound effect.
    void playerScored(int scorer) override;
    
    // Play the ball-bounce sound effect.
    void ballBounced() override;
    
private:
    // --------------- Constant declarations
    
    static const float PADDLE_MOVE_SPEED_MAX;
    static const float PADDLE_MOVE_SPEED_BASE;
    static const float PADDLE_MOVE_ACCEL;
    static const float PADDLE_HEIGHT;
    static const float PADDLE_THICKNESS;
    // how far the paddles are from the sides they defend
    static const float PADDLE_BACK_DISTANCE;
    
    static const float BALL_START_SPEED;
    static const float BALL_START_ANGLE_RANGE;
    
    // the time it takes a shown notification message to expire
    static const float NOTIF_LIFETIME;
    // how quickly a notification message fades after it expires
    static const float NOTIF_FADE_RATE;
    
    static const int SCORE_FONT_SIZE;
    static const int TITLE_FONT_SIZE;
    static const int CAPTION_FONT_SIZE;
    static const int TUTORIAL_FONT_SIZE;
    static const int CORNER_FONT_SIZE;
    
    static const std::string TITLE;
    static const std::string FONT_FILENAME;
    static const std::string BALL_SOUND_FILENAME;
    static const std::string MISS_SOUND_FILENAME;
    
    static const sf::Color BACKGROUND;
    static const sf::Color MENU_COVER;
    
    // either player gains this many points to win and end the game
    static const int POINTS_TO_WIN;
    
    // minimum and maximum values for difficulty
    static const int DIFFICULTY_MIN;
    static const int DIFFICULTY_MAX;
    
    // ----------------------------------
    
    // --------------- Fields and members for game data
    
    // main game window
    std::unique_ptr<sf::RenderWindow> window_;
    
    // sound buffer for effect made when the ball hits something
    sf::SoundBuffer ball_sound_;
    // sound buffer for effect made when the ball scores
    sf::SoundBuffer miss_sound_;
    // SFML object for playing sounds
    sf::Sound sound_player_;
    
    // display font
    sf::Font font_;
    
    // notification message text
    std::string notification_;
    // how long until the notification message starts fading
    float notification_life_ = 0;
    // how opaque is the notification message now
    float notification_opacity_ = 0;
    
    // clock for delta time
    sf::Clock frame_timer_;
    // time between the game launching and the previous frame
    sf::Time last_frame_;
    // time between the last and current frame
    sf::Time delta_;
    
    // is the game paused? are we in the menu?
    bool paused_ = true;
    
    // what difficulty level are we playing with? 0 is standard.
    // Positive indicates more difficult, negative indicates less.
    // Difficulty changes the time scale multiplier.
    int difficulty_ = 0;
    
    // whether or not an obstacle block should be added to the
    // center of the playing field.
    bool obstacle_ = false;
    
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
    
    // Draws an SFML text object at the given position in the given size.
    // If center_origin is true, the text object's origin will be
    // set to the center of its rectangular bounds.
    void drawText(sf::Text text, int font_size,
                  sf::Vector2f position, bool center_origin, bool bold);
    
    // Display a message to the middle of the play area, which 
    // fades over time while the game is unpaused.
    void showMessage(std::string message);
    
    void resetEntities();
    void resetScore();
    
    // Draw the pause-screen elements, including a translucent overlay.
    void drawMenu();
    
    // Draw play-area text elements, including score counters and possibly
    // a central notification message.
    void drawScore();
    
    // Return a multiplier to be applied to time delta to allow
    // pausing and slowdowns
    float getTimeScale();
    
    // Return the difficulty level's contribution to the time
    // scale multiplier.
    float getDifficultyTimeMultiplier();
    
    // Return a display-friendly name for the current difficulty level.
    std::string describeDifficulty();
    
    // Attempt to move a paddle according a multiplier determined
    // from human or AI input
    void applyPaddleInput(std::string paddle_id, float multiplier);
    
    // ------------------------
};

#endif // PONGGAME_H

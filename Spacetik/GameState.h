#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects.h"
#include <string>

class GameState {
private:
    int window_width;
    int window_height;
    int canvas_width;
    int canvas_height;
    int bounces;

    Ball* ball;
    Bar* player1;
    Bar* player2;

public:
    // Constructor
    //GameState(int windowWidth, int windowHeight, int canvasWidth, int canvasHeight, Ball* ball, Bar* player1, Bar* player2);

    // Destructor
    ~GameState();

    //// Getters
    //int get_window_width() const;
    //int get_window_height() const;
    //int get_canvas_width() const;
    //int get_canvas_height() const;
    //Ball& get_ball() const;
    //Bar& get_player1() const;
    //Bar& get_player2() const;
    //int get_bounces() const;

    //// Setters
    //void set_window_width(int width);
    //void set_window_height(int height);
    //void set_canvas_width(int width);
    //void set_canvas_height(int height);

    // Increment bounces
    void increment_bounces();

    // Static methods declared later
    static void drawBall();
    static void drawPlayers();
    static void drawText();
};

#endif // GAMESTATE_H
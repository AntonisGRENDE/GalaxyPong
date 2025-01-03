#pragma once

#include "graphics.h"
#include <random>
#include "vecmath.h"
#include "config.h"
using namespace std;
constexpr float MIN_DIRECTION_THRESHOLD = 0.3f;

class GameObjects {

protected:
	float center_x;
	float center_y;
	float speed;

public:
	GameObjects(float x, float y, float s) {
		center_x = x;
		center_y = y;
		speed = s;
	}

	float get_center_x() const {
		return center_x;
	}
	float get_center_y() const {
		return center_y;
	}
	float get_speed() const {
		return speed;
	}

	void set_center_x(float center_x) {
		this->center_x = center_x;
	}
	void set_center_y(float center_y) {
		this->center_y = center_y;
	}
	void set_speed(float speed) {
		this->speed = speed;
	}
};


class Ball: public GameObjects {

private:
	float ball_direction_x, ball_direction_y;
	float init_ball_center_x, init_ball_center_y;
	float init_ball_direction_x, init_ball_direction_y;
	float ball_radius;

public:
	Ball(float x, float y, float dx, float dy, float ball_radius, float ball_speed)
		: GameObjects(x, y, ball_speed){
		init_ball_center_x = x;
		init_ball_center_y = y;
		init_ball_direction_x = dx;
		init_ball_direction_y = dy;

		this->ball_radius = ball_radius;
		startAgain();
	}

	void startAgain() {
		center_x = init_ball_center_x;
		center_y = init_ball_center_y;

		// Random number generator for direction
		random_device seed;
		default_random_engine generator(seed());
		uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float dx, dy;

		do {
			dx = distribution(generator);
		} while (abs(dx) < MIN_DIRECTION_THRESHOLD);

		do {
			dy = distribution(generator);
		} while (abs(dy) < MIN_DIRECTION_THRESHOLD);

		// Normalize the direction vector
		float magnitude = sqrt(dx * dx + dy * dy);
		ball_direction_x = dx / magnitude;
		ball_direction_y = dy / magnitude;

		// Scale the direction to match the ball's speed
		ball_direction_x *= speed;
		ball_direction_y *= speed;
	}



	float get_ball_radius() const {
		return ball_radius;
	}
	float get_ball_direction_x() const {
		return ball_direction_x;
	}
	float get_ball_direction_y() const {
		return ball_direction_y;
	}


	void set_ball_direction_x(float ball_direction_x) {
		this->ball_direction_x = ball_direction_x;
	}
	void set_ball_direction_y(float ball_direction_y) {
		this->ball_direction_y = ball_direction_y;
	}
	void set_ball_radius(float ball_radius) {
		this->ball_radius = ball_radius;
	}
	void set_init_ball_direction_y(float init_ball_direction_y) {
		this->init_ball_direction_y = init_ball_direction_y;
	}
	void set_init_ball_direction_x(float init_ball_direction_x) {
		this->init_ball_direction_x = init_ball_direction_x;
	}

};


class Bar : public GameObjects{

protected:
	float width;
	float height;
	int points;

public:
	Bar(float x, float y, float w, float h, float speed)
		: GameObjects(x , y, speed){
		width = w;
		height = h;
		points = 0;
	}

	void moveup() {
		center_y = center_y - speed;
	}

	void movedown() {
		center_y = center_y + speed;
	}

	bool collisionWithBall(Ball& ball) {
		if (abs(center_x - ball.get_center_x()) - (width / 2) - (ball.get_ball_radius() - 1) < 0) {
			if (abs(center_y - ball.get_center_y()) - (height / 2) - (ball.get_ball_radius() - 1) < 0) {
				return true; // collision with ball took place
			}
		}
		return false;
	}

	void increment_points() {
		 ++points;
	}

	int get_points() {
		return points;
	}
	float get_width() {
		return width;
	}
	float get_height() {
		return height;
	}

};


class GameState {
protected:
	int window_width, window_height;
	float canvas_width, canvas_height;
	int bounces;

	Ball* ball;
	Bar* player1;
	Bar* player2;

public:
	GameState(int ww, int wh, float cw, float ch, Ball* ball, Bar* p1, Bar* p2)
		: ball(ball), player1(p1), player2(p2) {

		window_width = ww;
		window_height = wh;
		canvas_width = cw;
		canvas_height = ch;
		bounces = 0;
	}

	~GameState() {
		delete ball;
		delete player1;
		delete player2;
	}

	static void drawBall() {
		GameState* game = static_cast <GameState*>(graphics::getUserData());			// get all game data
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.0f;
		br.texture = ASSETS_PATH + "sun3.png";
		graphics::drawDisk(game->ball->get_center_x(), game->ball->get_center_y(), game->ball->get_ball_radius(), br);
	}

	static void drawPlayers() {
		GameState* game = reinterpret_cast <GameState*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.2f;
		br.texture = ASSETS_PATH + "bar3.png";
		graphics::drawRect(game->player1->get_center_x(), game->player1->get_center_y(), game->player1->get_width(), game->player1->get_height(), br);
		graphics::drawRect(game->player2->get_center_x(), game->player2->get_center_y(), game->player2->get_width(), game->player2->get_height(), br);
	}

	static void drawText() {
		GameState* game = reinterpret_cast <GameState*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_secondary_color[0] = 1.0f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 1.0f;
		graphics::drawText(2, 5, 5, "P1:" + to_string(game->player1->get_points()), br);
		graphics::drawText(game->canvas_width - 13, 5, 5, "  P2:" + to_string(game->player2->get_points()), br);
	}

	int get_window_width() {
		return window_width;
	}
	int get_window_height() {
		return window_height;
	}
	float get_canvas_width() {
		return canvas_width;
	}
	float get_canvas_height() {
		return canvas_height;
	}
	int get_bounces() {
		return bounces;
	}
	void increment_bounces() {
		++bounces;
	}

	Ball& get_ball() {
		return *ball;
	}
	Bar& get_player1() {
		return *player1;
	}
	Bar& get_player2() {
		return *player2;
	}


};
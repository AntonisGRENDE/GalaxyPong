
#include <random>
#include <string>
#include <iostream>
#include <cmath> //gia to apolyto abs

#include "graphics.h"
#include "vecmath.h"//an kanw sigkrouseis me ton tropo pou leei sto paradeigma
#include "config.h"
using namespace std;




class Ball {
private:
	float ball_center_x;
	float ball_center_y;
	float ball_direction_x;
	float ball_direction_y;
	float ball_radius;
	float ball_speed;


	float init_ball_center_x;
	float init_ball_center_y;
	float init_ball_direction_x;
	float init_ball_direction_y;
public:
	Ball(float x, float y, float dx, float dy, float br, float bs) { //default constructor
		init_ball_center_x = x;
		init_ball_center_y = y;
		init_ball_direction_x = dx;
		init_ball_direction_y = dy;

		ball_radius = br;
		ball_speed = bs;
		startAgain();
	}

	void startAgain() { //gets called in the start or every time the ball touches the sides
		ball_center_x = init_ball_center_x;
		ball_center_y = init_ball_center_y;

		// generates random numbers between -0.9999 to 0.9999
		random_device seed;
		default_random_engine generator(seed());
		uniform_real_distribution <float> distribution(-1.0, 1.0);
		float dx = distribution(generator);
		float dy = distribution(generator);

		// prokeimenou na min kanei polles anapidiseis sta plagia 
		while (dy < 0.3 && dy > -0.3) dy = distribution(generator);
		while (dx < 0.3 && dx > -0.3) dx = distribution(generator);
		ball_direction_x = dx;
		ball_direction_y = dy;

	}

	//getters
	float get_ball_center_x() {
		return ball_center_x;
	}
	float get_ball_center_y() {
		return ball_center_y;
	}
	float get_ball_speed() {
		return ball_speed;
	}
	float get_ball_radius() {
		return ball_radius;
	}
	float get_ball_direction_x() {
		return ball_direction_x;
	}
	float get_ball_direction_y() {
		return ball_direction_y;
	}
	//set
	void set_ball_center_x(float ball_center_x) {
		this->ball_center_x = ball_center_x;
	}
	void set_ball_center_y(float ball_center_y) {
		this->ball_center_y = ball_center_y;
	}
	void set_ball_speed(float bs) {
		ball_speed = bs;
	}
	void set_ball_direction_x(float ball_direction_x) {
		this->ball_direction_x = ball_direction_x;
	}
	void set_ball_direction_y(float ball_direction_y) {
		this->ball_direction_y = ball_direction_y;
	}
	void set_ball_radius(float br) {
		ball_radius = br;
	}
	void set_init_ball_direction_y(float dy) {
		init_ball_direction_y = dy;
	}
	void set_init_ball_direction_x(float dx) {
		init_ball_direction_x = dx;
	}

};


class Bar {
public:
	float center_x;
	float center_y;
	float width;
	float height;
	float speed;
	int points;

	Bar(float x, float y, float w, float h, float sp) {
		center_x = x;
		center_y = y;
		width = w;
		height = h;
		speed = sp;
		points = 0;

	}

	void moveup() {
		center_y = center_y - speed;
	}

	void movedown() {
		center_y = center_y + speed;
	}

	bool collisionWithBall(Ball& ball) {
		if (abs(center_x - ball.get_ball_center_x()) - (width / 2) - (ball.get_ball_radius() - 1) < 0) {
			if (abs(center_y - ball.get_ball_center_y()) - (height / 2) - (ball.get_ball_radius() - 1) < 0) {
				return true; //egine sigrousi me tin mpala
			}
		}
		return false;
	}

	float get_points() {
		return points;
	}
	float get_width() {
		return width;
	}
	float get_height() {
		return height;
	}

};


class Game {
public:
	int window_width;
	int window_height;
	int canvas_width;
	int canvas_height;
	int bounces;

	Ball& ball;
	Bar& player1;
	Bar& player2;

	Game(int ww, int wh, int cw, int ch, Ball& ball, Bar& p1, Bar& p2) :
		ball(ball), player1(p1), player2(p2) {
		window_width = ww;
		window_height = wh;
		canvas_width = cw;
		canvas_height = ch;
		bounces = 0;
	}

	static void drawBall() {

		Game* game = reinterpret_cast <Game*>(graphics::getUserData()); //pairneis ola ta stoixeia toy paixnidiou
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.0f;
		br.texture = assets_path + "sun3.png";
		graphics::drawDisk(game->ball.get_ball_center_x(), game->ball.get_ball_center_y(), game->ball.get_ball_radius(), br);
	}
	static void drawPlayers() {
		Game* game = reinterpret_cast <Game*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_opacity = 0.95f;
		br.outline_opacity = 0.2f;
		br.texture = assets_path + "bar3.png";
		graphics::drawRect(game->player1.center_x, game->player1.center_y, game->player1.width, game->player1.height, br);
		graphics::drawRect(game->player2.center_x, game->player2.center_y, game->player2.width, game->player2.height, br);
	}
	static void drawText() {
		Game* game = reinterpret_cast <Game*>(graphics::getUserData());
		graphics::Brush br;
		br.fill_secondary_color[0] = 1.0f;
		br.fill_secondary_color[1] = 1.0f;
		br.fill_secondary_color[2] = 1.0f;
		graphics::drawText(2, 5, 5, "P1:" + to_string(game->player1.points), br);
		graphics::drawText(game->canvas_width - 13, 5, 5, "  P2:" + to_string(game->player2.points), br);
	}

};



void draw() {
	graphics::Brush br;
	br.texture = assets_path + "galaxy3.png"; // analogia 2 pros tria peripou
	std::cout << "Galaxy path: " << assets_path + "galaxy3.png" << std::endl;
	br.outline_opacity = 0.0f;

	graphics::drawRect(CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_WIDTH + 150, CANVAS_WIDTH, br);
	Game::drawBall();
	Game::drawPlayers();
	Game::drawText();
}

void update(float ms) {
	//panw aristera gwnia einai 0,0 oi syntetagmenes
	Game* game = reinterpret_cast <Game*> (graphics::getUserData());
	Bar& player1 = game->player1;
	Bar& player2 = game->player2;


	// validate user input
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if ((player1.center_y - player1.height / 2) > 0.0f) {
			player1.moveup();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		if ((player1.center_y + player1.height / 2) < game->canvas_height) {
			player1.movedown();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		if ((player2.center_y - player2.height / 2) > 0.0f) {
			player2.moveup();
		}
	}
	//an to kentro tis mparas kai to miso toy mikoys exoun kseperasei
	if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		if ((player2.center_y + player2.height / 2) < game->canvas_height) {
			player2.movedown();
		}
	}

	bool play_collision_sound = false;
	Ball& ball = game->ball;
	//gia tin mpala
	if ((ball.get_ball_center_x() + ball.get_ball_radius()) >= game->canvas_width) { //xtipise deksia ara pontos
		std::string wav = assets_path + "point2.wav";
		graphics::playSound(wav, 0.5f);
		player1.points++;
		ball.startAgain();
	}
	else if ((ball.get_ball_center_x() - ball.get_ball_radius()) <= 0.0f) { //xtipise aristera ara pontos
		std::string wav = assets_path + "point2.wav";
		graphics::playSound(wav, 0.6f);
		player2.points++;
		ball.startAgain();
	}
	else if ((ball.get_ball_center_y() + ball.get_ball_radius()) >= game->canvas_height) { // xtipise pano 
		//kala tha itan na einai me tin epomeni thesi
		ball.set_ball_direction_y(-1 * ball.get_ball_direction_y());
		play_collision_sound = true;
	}
	else if ((ball.get_ball_center_y() - ball.get_ball_radius()) <= 0.0f) { // xtipise kato
		ball.set_ball_direction_y(-1 * ball.get_ball_direction_y());
		play_collision_sound = true;
	}
	else if (player1.collisionWithBall(ball)) {
		play_collision_sound = true;
		ball.set_ball_direction_x(-1 * ball.get_ball_direction_x());
	}
	else if (player2.collisionWithBall(ball)) {
		play_collision_sound = true;
		ball.set_ball_direction_x(-1 * ball.get_ball_direction_x());
	}
	if (play_collision_sound) {
		std::string wav = assets_path + "hit4.wav";
		graphics::playSound(wav, 0.7f);
		game->bounces++;
	}
	//nea thesi gia tin mpala
	ball.set_ball_center_x(ball.get_ball_center_x() + ball.get_ball_direction_x() * ball.get_ball_speed()); //+= ball.get_ball_direction_x() * ball.get_ball_speed();
	ball.set_ball_center_y(ball.get_ball_center_y() + ball.get_ball_direction_y() * ball.get_ball_speed()); //+= ball.get_ball_direction_y() * ball.get_ball_speed();
}



int main() {

	random_device seed;
	default_random_engine generator(seed());
	uniform_real_distribution <float> distribution(-1.0, 1.0);
	float dx = distribution(generator);
	float dy = distribution(generator);

	Ball ball(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, dx, dy, 4.0f, 0.7f);
	Bar player1(1.0f, CANVAS_HEIGHT / 2, 2.0, 20.0f, 0.9f);
	Bar player2(CANVAS_WIDTH - 1, CANVAS_HEIGHT / 2, 2.0f, 20.0f, 0.9f);
	Game game(WINDOW_WIDTH, WINDOW_HEIGHT, CANVAS_WIDTH, CANVAS_HEIGHT, ball, player1, player2); //den einai etoimi klasi

	graphics::createWindow(game.window_width, game.window_height, "Ergasia SGG");
	std::string mp3 = assets_path + "music.mp3";
	graphics::playSound(mp3, 1.0f); // play music

	graphics::setFont(assets_path + "orange juice 2.0.ttf");
	graphics::setUserData(&game);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);   //ti tha ginetai kathe fora poy tha ananewnetai h ekseliksi tou paixnidiou
	graphics::setCanvasSize(game.canvas_width, game.canvas_height);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);


	graphics::Brush br;
	br.fill_color[0] = 0.1f;
	br.fill_color[1] = 0.1f;
	br.fill_color[2] = 0.1f;
	graphics::setWindowBackground(br);


	graphics::startMessageLoop();
	graphics::destroyWindow();

	return 0;
}


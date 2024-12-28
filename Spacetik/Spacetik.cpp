
#include <random>
#include <string>
#include <iostream>
#include <cmath>
#include "GameComponents.cpp"

#include "graphics.h"
#include "vecmath.h"	//an kanw sigkrouseis me ton tropo pou leei sto paradeigma
#include "config.h"
using namespace std;



void draw() {
	graphics::Brush br;
	br.texture = assets_path + "galaxy3.png"; // ~ 2/3 ratio 
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
		if ((player1.get_center_y() - player1.height / 2) > 0.0f) {
			player1.moveup();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		if ((player1.get_center_y() + player1.height / 2) < game->canvas_height) {
			player1.movedown();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		if ((player2.get_center_y() - player2.height / 2) > 0.0f) {
			player2.moveup();
		}
	}
	//an to kentro tis mparas kai to miso toy mikoys exoun kseperasei
	if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		if ((player2.get_center_y() + player2.height / 2) < game->canvas_height) {
			player2.movedown();
		}
	}

	bool play_collision_sound = false;
	Ball& ball = game->ball;
	//gia tin mpala
	if ((ball.get_center_x() + ball.get_ball_radius()) >= game->canvas_width) { //xtipise deksia ara pontos
		std::string wav = assets_path + "point2.wav";
		graphics::playSound(wav, 0.5f);
		player1.points++;
		ball.startAgain();
	}
	else if ((ball.get_center_x() - ball.get_ball_radius()) <= 0.0f) { //xtipise aristera ara pontos
		std::string wav = assets_path + "point2.wav";
		graphics::playSound(wav, 0.6f);
		player2.points++;
		ball.startAgain();
	}
	else if ((ball.get_center_y() + ball.get_ball_radius()) >= game->canvas_height) { // xtipise pano 
		//kala tha itan na einai me tin epomeni thesi
		ball.set_ball_direction_y(-1 * ball.get_ball_direction_y());
		play_collision_sound = true;
	}
	else if ((ball.get_center_y() - ball.get_ball_radius()) <= 0.0f) { // xtipise kato
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
	ball.set_center_x(ball.get_center_x() + ball.get_ball_direction_x() * ball.get_speed()); //+= ball.get_ball_direction_x() * ball.get_ball_speed();
	ball.set_center_y(ball.get_center_y() + ball.get_ball_direction_y() * ball.get_speed()); //+= ball.get_ball_direction_y() * ball.get_ball_speed();
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

	graphics::createWindow(game.window_width, game.window_height, "SGG Assingment");
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


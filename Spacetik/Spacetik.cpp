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
	br.texture = ASSETS_PATH + "galaxy3.png"; // ~ 2/3 ratio 
	br.outline_opacity = 0.0f;

	graphics::drawRect(CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_WIDTH + 150, CANVAS_WIDTH, br);
	Game::drawBall();
	Game::drawPlayers();
	Game::drawText();
}


void checkUserInput(Bar* player1, Bar* player2, Game* game) {
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if ((player1->get_center_y() - player1->get_height() / 2) > 0.0f) {
			player1->moveup();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		if ((player1->get_center_y() + player1->get_height() / 2) < game->get_canvas_height()) {
			player1->movedown();
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		if ((player2->get_center_y() - player2->get_height() / 2) > 0.0f) {
			player2->moveup();
		}
	}
	// an to kentro tis mparas kai to miso toy mikoys exoun kseperasei
	if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		if ((player2->get_center_y() + player2->get_height() / 2) < game->get_canvas_height()) {
			player2->movedown();
		}
	}
}


void checkCollision(Bar* player1, Bar* player2, Ball* ball, Game* game) {
	bool play_collision_sound = false;

	//gia tin mpala
	if ((ball->get_center_x() + ball->get_ball_radius()) >= game->get_canvas_width()) { //xtipise deksia ara pontos
		std::string wav = ASSETS_PATH + "point2.wav";
		graphics::playSound(wav, 0.5f);
		player1->increment_points();
		ball->startAgain();
	}
	else if ((ball->get_center_x() - ball->get_ball_radius()) <= 0.0f) { //xtipise aristera ara pontos
		std::string wav = ASSETS_PATH + "point2.wav";
		graphics::playSound(wav, 0.6f);
		player2->increment_points();
		ball->startAgain();
	}
	else if ((ball->get_center_y() + ball->get_ball_radius()) >= game->get_canvas_height()) { // xtipise pano 
		//kala tha itan na einai me tin epomeni thesi
		ball->set_ball_direction_y(-1 * ball->get_ball_direction_y());
		play_collision_sound = true;
	}
	else if ((ball->get_center_y() - ball->get_ball_radius()) <= 0.0f) { // xtipise kato
		ball->set_ball_direction_y(-1 * ball->get_ball_direction_y());
		play_collision_sound = true;
	}
	else if (player1->collisionWithBall(*ball)) {
		play_collision_sound = true;
		ball->set_ball_direction_x(-1 * ball->get_ball_direction_x());
	}
	else if (player2->collisionWithBall(*ball)) {
		play_collision_sound = true;
		ball->set_ball_direction_x(-1 * ball->get_ball_direction_x());
	}
	if (play_collision_sound) {
		std::string wav = ASSETS_PATH + "hit4.wav";
		graphics::playSound(wav, 0.7f);
		game->increment_bounces();
	}

	// new ball position
	ball->set_center_x(ball->get_center_x() + ball->get_ball_direction_x() * ball->get_speed()); //+= ball.get_ball_direction_x() * ball.get_ball_speed();
	ball->set_center_y(ball->get_center_y() + ball->get_ball_direction_y() * ball->get_speed()); //+= ball.get_ball_direction_y() * ball.get_ball_speed();
}

void update(float ms) {
	// panw aristera gwnia einai 0,0 oi syntetagmenes
	Game* game = reinterpret_cast <Game*> (graphics::getUserData());
	if (!game) return;
	Bar* player1 = &game->get_player1();
	Bar* player2 = &game->get_player2();
	Ball* ball   = &game->get_ball();


	// validate user input
	checkUserInput(player1, player2, game);
	checkCollision(player1,player2,ball,game);
}


int main() {

	random_device seed;
	default_random_engine generator(seed());
	uniform_real_distribution <float> distribution(-1.0, 1.0);
	float dx = distribution(generator);
	float dy = distribution(generator);

	Ball* ball   = new Ball(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, dx, dy, 4.0f, 0.7f);
	Bar* player1 = new Bar(1.0f, CANVAS_HEIGHT / 2, 2.0, 20.0f, 0.9f);
	Bar* player2 = new Bar(CANVAS_WIDTH - 1, CANVAS_HEIGHT / 2, 2.0f, 20.0f, 0.9f);
	Game* game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT, CANVAS_WIDTH, CANVAS_HEIGHT, ball, player1, player2); // Now game is on the heap

	graphics::createWindow((*game).get_window_width(), (*game).get_window_height(), "SGG Assingment");
	std::string mp3 = ASSETS_PATH + "music.mp3";
	graphics::playSound(mp3, 1.0f); // play music

	graphics::setFont(ASSETS_PATH + "orange juice 2.0.ttf");
	graphics::setUserData(game);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);   //ti tha ginetai kathe fora poy tha ananewnetai h ekseliksi tou paixnidiou
	graphics::setCanvasSize((*game).get_canvas_width(), (*game).get_canvas_height());
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);


	graphics::Brush br;
	br.fill_color[0] = 0.1f;
	br.fill_color[1] = 0.1f;
	br.fill_color[2] = 0.1f;
	graphics::setWindowBackground(br);


	graphics::startMessageLoop();
	graphics::destroyWindow();
	delete player1, player2, ball, game;
	return 0;
}


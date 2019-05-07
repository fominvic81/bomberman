#include <windows.h>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#include "network.h"
#include "server.h"
#include "packets.h"

using namespace std;
using namespace sf;


//	View view;
//	view.setCenter(Vector2f(x, y));
//	view.setSize(Vector2f(w, h));
//	app.setView(view);






//		CircleShape button1;
//		button1.setPosition(100, 100);
//		button1.setRadius(35);
//		button1.setFillColor(Color(50, 50, 200));
//		app.draw(button1);





//		if (Keyboard::isKeyPressed(Keyboard::Left)) {
//			x -= dt * 100;
//		}
//		if (Keyboard::isKeyPressed(Keyboard::Right)) {
//			x += dt * 100;
//		}
//		if (Keyboard::isKeyPressed(Keyboard::Up)) {
//			y -= dt * 100;
//		}
//		if (Keyboard::isKeyPressed(Keyboard::Down)) {
//			y += dt * 100;
//		}



//	RectangleShape cube1;
//	cube1.setPosition(x-400, y-300);
//	cube1.setSize(Vector2f(inv_power/850, 15));
//	cube1.setFillColor(Color(40, 200, 10));
//	cube1.setOutlineThickness(2);
//	cube1.setOutlineColor(sf::Color(255, 255, 255));
//	app.draw(cube1);

//	Text inv_text;
//	inv_text.setPosition(x-400, y-290);
//	inv_text.setPosition(50, 100);
//	inv_text.setString("invisibility");
//	inv_text.setFillColor(Color(10, 200, 250));
//	inv_text.setCharacterSize(20);
//	inv_text.setStyle(Text::Bold | Text::Underlined);
//	inv_text.setFont(font);
//	app.draw(inv_text);

extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__attribute__((dllexport)) DWORD NvOptimusEnablement = 0x00000001;
}

RenderWindow app(
		VideoMode(800, 600),
		"Test",
		Style::Titlebar | Style::Close | Style::Resize,
		ContextSettings(0, 0, 8)
);
Clock timer;
Clock gametimer;
Font font;
Texture texture_hardbrick;
Texture texture_grass;
Texture texture_bush;
Texture texture_ice;
Texture texture_saw;
Texture texture_floorsaw;
Texture texture_health;
Texture texture_redbrick;
Texture texture_bomb;
Texture texture_oldgrass;
Texture texture_player;
Texture texture_boom[9];
Texture texture_whitebrick;
Texture texture_whitehardbrick;
Texture texture_pit;



struct Playercl {
	float x = 300, y = 300, retotate=0;
	bool isCreate = false;
	int team = 0,speed = 100,radius = 25,health = 0,maxhealth = 10000,maxbombcount = 1,powerboom = 1,id;
	Clock respawntimer,bombtimer;
	string name;
	int lname, lvl, maprendrad = 16;
	int move = 0;
} player;

struct Playerscl {
	float x = 0, y = 0, retotate=0;
	bool isCreate = false;
	int team = 0, radius = 25, health = 2 ,id;
	string name;
	int lname, lvl;
} playerscl[32];


struct bombcl {
	float x = 2, y = 2;
	Clock timerboom;
	int author = 1,powerboom, team = 0;
	bool isCreate = false;
} bombscl[2000];

enum class BoomDirection : int {
	CENTER = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4,
};

struct boomcl {
	float x = 0, y = 0;
	int team = 0, author,powerboom;
	bool isCreate = false;
	Clock btimer;
	BoomDirection direction;
} boomscl[10000];


//int *test_map2[] = {
//		//      1  2  3  4  5  6  7  8  9  10 125 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
//		(int[]){25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
//		(int[]){25, 2,  2,  2,  2,  0,  50, 50, 50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 50, 50, 2,  2,  2,  2,  2,  25},
//		(int[]){25, 2,  0,  0,  0,  0,  50, 50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 50, 0,  0,  0,  0,  2,  25},
//		(int[]){25, 2,  0,  3,  0,  0,  50, 0,  0,  0,  50, 50, 50, 50, 1,  1,  1,  1,  50, 50, 50, 50, 0,  0,  0,  50, 0,  0,  3,  0,  2,  25},
//		(int[]){25, 2,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  50, 1,  1,  1,  1,  50, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  2,  25},
//		(int[]){25, 0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  2,  25},
//		(int[]){25, 50, 50, 50, 0,  0,  50, 50, 50, 50, 50, 0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  50, 50, 50, 50, 50, 0,  0,  50, 50, 50, 25},
//		(int[]){25, 50, 50,  0, 0,  0,  50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 0,  0,  0,  50, 50, 25},
//		(int[]){25, 50, 0,  0,  0,  0,  50, 0,  6,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  6,  0,  50, 0,  0,  0,  0,  50, 25},
//		(int[]){25, 0,  0,  0,  0,  0,  50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 0,  0,  0,  0,  0,  25},
//		(int[]){25, 0,  0,  50, 50, 50, 50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 50, 50, 50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 50, 1,  1,  1,  1,  1,  1,  1,  1,  1,  4,  4,  4,  4,  1,  1,  1,  1,  1,  1,  1,  1,  1,  50, 50, 0,  0,  25},
//		(int[]){25, 0,  0,  1,  1,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  1,  1,  0,  0,  25},
//		(int[]){25, 0,  0,  1,  1,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  1,  1,  0,  0,  25},
//		(int[]){25, 0,  0,  1,  1,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  1,  1,  0,  0,  25},
//		(int[]){25, 0,  0,  1,  1,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  1,  1,  0,  0,  25},
//		(int[]){25, 0,  0,  50, 50, 1,  1,  1,  1,  1,  1,  1,  1,  1,  4,  4,  4,  4,  1,  1,  1,  1,  1,  1,  1,  1,  1,  50, 50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 0,  0,  25},
//		(int[]){25, 0,  0,  50, 50, 50, 50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 50, 50, 50, 0,  0,  25},
//		(int[]){25, 0,  0,  0,  0,  0,  50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 0,  0,  0,  0,  0,  25},
//		(int[]){25, 50, 0,  0,  0,  0,  50, 0,  6,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  6,  0,  50, 0,  0,  0,  0,  50, 25},
//		(int[]){25, 50, 50, 0,  0,  0,  50, 0,  0,  0,  0,  0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  0,  0,  0,  0,  50, 0,  0,  0,  50, 50, 25},
//		(int[]){25, 50, 50, 50, 0,  0,  50, 50, 50, 50, 50, 0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  50, 50, 50, 50, 50, 0,  0,  50, 50, 50, 25},
//		(int[]){25, 2,  0,  0,  0,  7,  7,  0,  0,  0,  50, 0,  0,  1,  4,  4,  4,  4,  1,  0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  2,  25},
//		(int[]){25, 2,  0,  0,  0,  7,  7,  0,  0,  0,  50, 0,  0,  50, 1,  1,  1,  1,  50, 0,  0,  50, 0,  0,  0,  0,  0,  0,  0,  0,  2,  25},
//		(int[]){25, 2,  0,  3,  0,  0,  50, 0,  0,  0,  50, 50, 50, 50, 1,  1,  1,  1,  50, 50, 50, 50, 0,  0,  0,  50, 0,  0,  3,  0,  2,  25},
//		(int[]){25, 2,  0,  0,  0,  0,  50, 50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 50, 0,  0,  0,  0,  2,  25},
//		(int[]){25, 2,  2,  2,  2,  2,  50, 50, 50, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  50, 50, 50, 2,  2,  2,  2,  2,  25},
//		(int[]){25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
//};
//
//
//int test_map[8][8] = {
//int *test_map[] = {
//		(int[]){1, 1, 1, 1, 1, 1, 1, 1},
//		(int[]){1, 0, 0, 0, 0, 0, 0, 1},
//		(int[]){1, 0, 0, 1, 1, 0, 0, 1},
//		(int[]){1, 0, 1, 1, 1, 1, 0, 1},
//		(int[]){1, 0, 1, 1, 1, 1, 0, 1},
//		(int[]){1, 0, 0, 1, 1, 0, 0, 1},
//		(int[]){1, 0, 0, 0, 0, 0, 0, 1},
//		(int[]){1, 1, 1, 1, 1, 1, 1, 1},
//};


int mapscl[1024][1024];
int sizemcl = 0;

void createBoomUpCl(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

//	for (int i = 0; i <= 9999; ++i) {
//		if (boomscl[i].isCreate) {
//			continue;
//		}
//		boomscl[i].author = u;
//		boomscl[i].x = x;
//		boomscl[i].y = y;
//		boomscl[i].isCreate = true;
//		boomscl[i].powerboom = powerboom;
//		boomscl[i].team = player.team;
//		boomscl[i].direction = BoomDirection::UP;
//		boomscl[i].btimer.restart();
//
//		if (powerboom > 0) {
//			createBoomUpCl(u, powerboom-1, x, y-50);
//		}
//
//		break;
//	}
  for (auto &boom : boomscl) {
    if (boom.isCreate) {
      continue;
    }
    boom.author = u;
    boom.x = x;
    boom.y = y;
    boom.isCreate = true;
    boom.powerboom = powerboom;
    boom.team = player.team;
    boom.direction = BoomDirection::UP;
    boom.btimer.restart();

    if (powerboom > 0) {
      createBoomUpCl(u, powerboom-1, x, y-50);
    }

    break;
  }
}

void createBoomDownCl(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

	for (int i = 0; i <= 9999; ++i) {
		if (boomscl[i].isCreate) {
			continue;
		}
		boomscl[i].author = u;
		boomscl[i].x = x;
		boomscl[i].y = y;
		boomscl[i].isCreate = true;
		boomscl[i].powerboom = powerboom;
		boomscl[i].team = player.team;
		boomscl[i].direction = BoomDirection::DOWN;
		boomscl[i].btimer.restart();

		if (powerboom > 0) {
			createBoomDownCl(u, powerboom-1, x, y+50);
		}

		break;
	}
}

void createBoomRightCl(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

	for (int i = 0; i <= 9999; ++i) {
		if (boomscl[i].isCreate) {
			continue;
		}
		boomscl[i].author = u;
		boomscl[i].x = x;
		boomscl[i].y = y;
		boomscl[i].isCreate = true;
		boomscl[i].powerboom = powerboom;
		boomscl[i].team = player.team;
		boomscl[i].direction = BoomDirection::RIGHT;
		boomscl[i].btimer.restart();

		if (powerboom > 0) {
			createBoomRightCl(u, powerboom-1, x+50, y);
		}

		break;
	}
}

void createBoomLeftCl(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

	for (int i = 0; i <= 9999; ++i) {
		if (boomscl[i].isCreate) {
			continue;
		}
		boomscl[i].author = u;
		boomscl[i].x = x;
		boomscl[i].y = y;
		boomscl[i].isCreate = true;
		boomscl[i].powerboom = powerboom;
		boomscl[i].team = player.team;
		boomscl[i].direction = BoomDirection::LEFT;
		boomscl[i].btimer.restart();

		if (powerboom > 0) {
			createBoomLeftCl(u, powerboom-1, x-50, y);
		}

		break;
	}
}

void createBoomCenterCl(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

	y += 25;

	for (int i = 0; i <= 9999; ++i) {
		if (boomscl[i].isCreate) {
			continue;
		}
		boomscl[i].author = u;
		boomscl[i].x = x;
		boomscl[i].y = y;
		boomscl[i].isCreate = true;
		boomscl[i].powerboom = powerboom;
		boomscl[i].team = player.team;
		boomscl[i].direction = BoomDirection::CENTER;
		boomscl[i].btimer.restart();

		if (powerboom > 0) {
			createBoomUpCl(u, powerboom-1, x, y-50);
			createBoomDownCl(u, powerboom-1, x, y+50);
			createBoomRightCl(u, powerboom-1, x+50, y);
			createBoomLeftCl(u, powerboom-1, x-50, y);
		}

		break;
	}
}


void createBomb(float x, float y, int i, int powerboom) {

		bombscl[i].x = x;
		bombscl[i].y = y;
		bombscl[i].isCreate = true;
		bombscl[i].powerboom = powerboom;
		bombscl[i].timerboom.restart();
}






void render_bomb() {



	for (int i = 0; i < 1999; ++i) {

		if (!bombscl[i].isCreate) {
			continue;
		}

		if (bombscl[i].timerboom.getElapsedTime().asSeconds() >= 2.3) {
			createBoomCenterCl(bombscl[i].author, bombscl[i].powerboom, bombscl[i].x, bombscl[i].y);

			++player.maxbombcount;
			bombscl[i].isCreate = false;
		}

		RectangleShape bomb;
		bomb.setPosition(bombscl[i].x, bombscl[i].y);
		bomb.setSize(Vector2f(50, 100));
		bomb.setTexture(&texture_bomb);
		int a = (((int)bombscl[i].timerboom.getElapsedTime().asMilliseconds() / 200) % 4) * 32;
		bomb.setTextureRect(IntRect(a, 0, 32, 64));
		app.draw(bomb);


	}

}

void render_boom() {

	for (int i = 0; i <= 9999; ++i) {
		if (!boomscl[i].isCreate) {
			continue;
		}

		int speed = 40;

		if (boomscl[i].btimer.getElapsedTime().asMilliseconds() >= speed * 6) {
			boomscl[i].isCreate = false;
		}

		RectangleShape boom_;
		boom_.setPosition(boomscl[i].x, boomscl[i].y);
		boom_.setSize(Vector2f(50, 50));

		int texture_id = (int) boomscl[i].direction;
		if (boomscl[i].direction != BoomDirection::CENTER && boomscl[i].powerboom == 0) {
			texture_id += 4;
		}
		boom_.setTexture(&texture_boom[texture_id]);


		int a = (((int) boomscl[i].btimer.getElapsedTime().asMilliseconds() / speed) % 6) * 32;
		boom_.setTextureRect(IntRect(a, 0, 32, 32));

		app.draw(boom_);


//		if (boomscl[i].direction == 0) { //CENTER
//
//			continue;
//		}
//
//		if (boomscl[i].direction == 1) { //UP
//
//			continue;
//		}
//
//		if (boomscl[i].direction == 2) { //DOWN
//
//			continue;
//		}
//
//		if (boomscl[i].direction == 3) { //RIGHT
//
//			continue;
//		}
//
//		if (boomscl[i].direction == 4) { //LEFT
//
//			continue;
//		}

	}

}

void playerController(int direction, float dt) {

	float w = app.getSize().x;
	float h = app.getSize().y;
	float _if;

//	bool touch = false;
//  playerscl[playerNumber].radius/


	if (direction == 1) { // LEFT

		_if = mapscl[(int)floor(player.x/50)][(int)floor(player.y/50)];
		if (_if >= 25) {
			player.x += dt * player.speed;
		}


		_if = mapscl[(int)floor(player.x/50)][(int)floor(player.y/50+player.radius/33.33333)];
		if (_if >= 25) {
			player.x += dt * player.speed;
		}


		player.x -= dt * player.speed;
		player.retotate = 180;
	}
	if (direction == 2) { // RIGHT

		_if = mapscl[(int)floor(player.x/50 + player.radius*0.04)][(int)floor(player.y/50)];
		if (_if >= 25) {
			player.x -= dt * player.speed;
		}



		_if = mapscl[(int)floor(player.x/50 + player.radius*0.04)][(int)floor(player.y/50+player.radius/33.33333)];
		if (_if >= 25) {
			player.x -= dt * player.speed;
		}



		player.x += dt * player.speed;
		player.retotate = 0;
	}
	if (direction == 3) { // UP

		_if = mapscl[(int)floor(player.x/50+0.01)][(int)floor(player.y/50-0.01)];
		if (_if >= 25) {
			player.y += dt * player.speed;
		}



		_if = mapscl[(int)floor(player.x/50+player.radius/30.30303)][(int)floor(player.y/50-0.01)];
		if (_if >= 25) {
			player.y += dt * player.speed;
		}



		player.y -= dt * player.speed;
		player.retotate = 270;
	}
	if (direction == 4) { // DOWN

		_if = mapscl[(int)floor(player.x/50+0.01)][(int)floor(player.y/50+player.radius*0.04)];
		if (_if >= 25) {
			player.y -= dt * player.speed;
		}



		_if = mapscl[(int)floor(player.x/50+player.radius/33.33333)][(int)floor(player.y/50+player.radius*0.04)];
		if (_if >= 25) {
			player.y -= dt * player.speed;
		}



		player.y += dt * player.speed;
		player.retotate = 90;
	}

	++player.move;

}

void render_players() {
  for (int i = 0; i < 32; ++i) {

    if (playerscl[i].health <= 0) continue;

//    cout << "-1 " << i << "\n";

    if (!playerscl[i].isCreate) continue;

    cout << "-2 " << i << "\n";

    RectangleShape player_;

    player_.setTexture(&texture_player);
    player_.setTextureRect(IntRect(0, 20, 253, 176));
//		player_].setPosition(player.x, players.y);
    player_.setSize(Vector2f(playerscl[i].radius*2,playerscl[i].radius*2));
    player_.setRotation(0);

    player_.move(playerscl[i].x, playerscl[i].y);
    player_.move(player_.getSize() / 2.f);
    player_.setOrigin(player_.getSize() / 2.f);
    player_.rotate(player.retotate);

    app.draw(player_);

  }
}


void render_player() {

	RectangleShape player_;

	float w = app.getSize().x;
	float h = app.getSize().y;

	if (player.health <= 0) {
		return;
	}

	float s = 1;

	player_.setTexture(&texture_player);
	player_.setTextureRect(IntRect(0, 20, 253, 176));
//		player_].setPosition(player.x, players.y);
	player_.setSize(Vector2f(player.radius*2,player.radius*2));
	player_.setRotation(0);

	player_.move(player.x, player.y);
	player_.move(player_.getSize() / 2.f);
	player_.setOrigin(player_.getSize() / 2.f);
	player_.rotate(player.retotate);



	View view;
	view.setCenter(Vector2f(player.x, player.y));
	view.setSize(Vector2f(w, h));
	app.setView(view);



	if (player.team == 0) {
		player_.setFillColor(Color(255,50,50));
	}

	if (player.team == 1) {
		player_.setFillColor(Color(255,50,50));
	}

	if (player.team == 2) {
		player_.setFillColor(Color(50,255,50));
	}

	if (player.team == 3) {
		player_.setFillColor(Color(50,50,255));
	}

	if (player.team == 4) {
		player_.setFillColor(Color(50,50,50));
	}



	app.draw(player_);


	RectangleShape playerhl;
	RectangleShape playerhl1;



	playerhl1.setPosition(player.x, player.y - 10);

	playerhl1.setSize(Vector2f(50, 4));
	playerhl1.setFillColor(sf::Color(75, 25, 75));

	playerhl1.setOutlineThickness(2);
	playerhl1.setOutlineColor(sf::Color(210, 150, 150));


	app.draw(playerhl1);


	playerhl.setPosition(player.x, player.y - 10);

	playerhl.setSize(Vector2f((float)player.health/(float)player.maxhealth*50, 4));
	playerhl.setFillColor(sf::Color(25, 250, 10));

//	playerhl.setOutlineThickness(2);
//	playerhl.setOutlineColor(sf::Color(250, 150, 100));


	app.draw(playerhl);
	if (Keyboard::isKeyPressed(Keyboard::LShift)) {
		player.health -= 10;
	}

}



void render_map() {

	int rad = player.maprendrad;

	int si = max(0, (int)(player.x) / 50 - rad);
	int fi = min(sizemcl, (int)(player.x) / 50 + rad);

	int sj = max(0, (int)(player.y) / 50 - rad);
	int fj = min(sizemcl, (int)(player.y) / 50 + rad);

	for (int i = si; i < fi; ++i) {
		for (int j = sj; j < fj; ++j) {
			RectangleShape block;

			int i1 = i*50, j1 = j*50;

      block.setPosition(i1-1, j1-1);
      block.setSize(Vector2f(52, 52));
      block.setTexture(&texture_grass);
      block.setTextureRect(IntRect(0, 0, 100, 100));
      app.draw(block);

			if (mapscl[i][j] == 0) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_grass);
				block.setTextureRect(IntRect(0, 0, 100, 100));
				app.draw(block);

				continue;
			}

			if (mapscl[i][j] == 2) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_ice);
				block.setTextureRect(IntRect(15, 15, 130, 130));
				app.draw(block);

				continue;
			}

			if (mapscl[i][j] == 3) {

        block.setPosition(i1-1, j1-1);
        block.setSize(Vector2f(52, 52));
        block.setTexture(&texture_pit);
        block.setTextureRect(IntRect(0, 0, 128, 128));
        app.draw(block);

				continue;
			}

			if (mapscl[i][j] == 4) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_oldgrass);
				block.setTextureRect(IntRect(0, 0, 225, 225));
				app.draw(block);

				continue;
			}

			if (mapscl[i][j] == 5) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_floorsaw);
				block.setTextureRect(IntRect(0, 0, 37, 37));
				app.draw(block);

				int a = (((int)bombscl[i].timerboom.getElapsedTime().asMilliseconds()) % 360);

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_saw);
				block.setTextureRect(IntRect(0, 0, 40, 40));

				block.move(block.getSize() / 2.f);
				block.setOrigin(block.getSize() / 2.f);
				block.rotate(a);
				app.draw(block);

//				block.setOrigin(0, 0);
//				block.setRotation(0);


				continue;
			}

			if (mapscl[i][j] == 25) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_hardbrick);
				block.setTextureRect(IntRect(0, 0, 128, 128));
				app.draw(block);

				continue;
			}

      if (mapscl[i][j] == 30) {

        block.setPosition(i1-1, j1-1);
        block.setSize(Vector2f(52, 52));
        block.setTexture(&texture_whitehardbrick);
        block.setTextureRect(IntRect(0, 0, 128, 128));
        app.draw(block);

        continue;
      }

			if (mapscl[i][j] == 50) {

				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_whitebrick);
				block.setTextureRect(IntRect(0, 0, 128, 128));
				app.draw(block);

				continue;
			}
		}
	}
}

void render_map2() {
//  app.getView()

	int rad = player.maprendrad;

	int si = max(0, (int)(player.x) / 50 - rad);
	int fi = min(sizemcl, (int)(player.x) / 50 + rad);

	int sj = max(0, (int)(player.y) / 50 - rad);
	int fj = min(sizemcl, (int)(player.y) / 50 + rad);

	for (int i = si; i < fi; ++i) {
		for (int j = sj; j < fj; ++j) {

			RectangleShape block;

			int i1 = i*50, j1 = j*50;

			if (mapscl[i][j] == 1) {


				block.setPosition(i1-1, j1-1);
				block.setSize(Vector2f(52, 52));
				block.setTexture(&texture_bush);
				block.setTextureRect(IntRect(0, 0, 128, 128));
				app.draw(block);

				continue;
			}

		}
	}

}


void setup() {
	player.isCreate = true;
	player.team = 0;

	font.loadFromFile(R"(C:\Windows\Fonts\segoeui.ttf)");
	texture_grass.loadFromFile(R"(D:\Vitya\images\lightgreengrass.jpg)");
  texture_ice.loadFromFile(R"(D:\Vitya\images\ice.jpg)");
  texture_bush.loadFromFile(R"(D:\Vitya\images\bush.jpeg)");
  texture_redbrick.loadFromFile(R"(D:\Vitya\images\redbrick.jpg)");
  texture_bomb.loadFromFile(R"(D:\Vitya\images\bomb.png)");
  texture_oldgrass.loadFromFile(R"(D:\Vitya\images\oldgrass1.jpg)");
  texture_saw.loadFromFile(R"(D:\Vitya\images\saw.png)");
  texture_floorsaw.loadFromFile(R"(D:\Vitya\images\floorsaw.png)");
  texture_player.loadFromFile(R"(D:\Vitya\images\player.gif)");
  texture_hardbrick.loadFromFile(R"(D:\Vitya\images\hardbrick.jpg)");
  texture_whitebrick.loadFromFile(R"(D:\Vitya\images\whitebrick.png)");
  texture_whitehardbrick.loadFromFile(R"(D:\Vitya\images\whitehardbrick.png)");
  texture_pit.loadFromFile(R"(D:\Vitya\images\pit.png)");


  texture_boom[(int)BoomDirection::CENTER].loadFromFile(R"(D:\Vitya\images\explos_0.png)");
	texture_boom[(int)BoomDirection::UP].loadFromFile(R"(D:\Vitya\images\explos_up.png)");
	texture_boom[(int)BoomDirection::DOWN].loadFromFile(R"(D:\Vitya\images\explos_dp.png)");
	texture_boom[(int)BoomDirection::LEFT].loadFromFile(R"(D:\Vitya\images\explos_lp.png)");
	texture_boom[(int)BoomDirection::RIGHT].loadFromFile(R"(D:\Vitya\images\explos_rp.png)");

	texture_boom[(int)BoomDirection::UP + 4].loadFromFile(R"(D:\Vitya\images\explos_u.png)");
	texture_boom[(int)BoomDirection::DOWN + 4].loadFromFile(R"(D:\Vitya\images\explos_d.png)");
	texture_boom[(int)BoomDirection::LEFT + 4].loadFromFile(R"(D:\Vitya\images\explos_l.png)");
	texture_boom[(int)BoomDirection::RIGHT + 4].loadFromFile(R"(D:\Vitya\images\explos_r.png)");
}

void events() {
	Event event;
	while (app.pollEvent(event)) {
		if (event.type == Event::Closed) {
			app.close();
		} else if (event.type == Event::KeyPressed) {
//				cout << "Control: " << event.key.control << endl;
//				cout << "Shift: " << event.key.shift << endl;
//				cout << "Alt: " << event.key.alt << endl;
//				cout << "Code: " << event.key.code << endl;

			if (event.key.code == Keyboard::Escape) {
				app.close();
			}
		} else if (event.type == Event::KeyReleased) {

		}
	}
}


void controller() {
// position = position.linear_interpolate(desired_position, 1 - exp(-20 * dt))
//  float x1, y1, x2, y2;
//
//  x1 = x1 + (x2 - x1) * (1 - exp(-20 * dt));
//  y1 = y1 + (y2 - y1) * (1 - exp(-20 * dt));


	float w = app.getSize().x;
	float h = app.getSize().y;
	float dt = timer.getElapsedTime().asSeconds();
	timer.restart();

	app.clear(Color(0, 0, 0));

	if (app.hasFocus()) {
    if (Keyboard::isKeyPressed(Keyboard::LControl)) {
      if (player.bombtimer.getElapsedTime().asSeconds() >= 0.25) {

        PacketPlayerCreateBomb bomb1;
        bomb1.id = player.id;

        network_client_send(&bomb1);
        player.bombtimer.restart();
      }


    }
    if (Keyboard::isKeyPressed(Keyboard::LShift)) {
      player.x = 100;
      player.y = 100;
    }

    if (Keyboard::isKeyPressed(Keyboard::Left)) {
      playerController(1, dt);
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      playerController(2, dt);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
      playerController(3, dt);
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      playerController(4, dt);
    }
  }

	if (player.move > 7) {
    PacketPlayerMyCoordinates player_coord;

    player_coord.id = player.id;
    player_coord.x = player.x;
    player_coord.y = player.y;

    network_client_send(&player_coord);

    player.move = 0;
  }

	render_map();
	render_player();
  render_players();
  render_bomb();
  render_boom();
  render_map2();

  View view;
  view.setCenter(Vector2f(player.x, player.y));
  view.setSize(Vector2f(w, h));
  app.setView(view);

	app.display();
}




void network_on_client_connected() {

	PacketPlayerConnect new_pl;
	new_pl.lmyName = player.lname;
	for (int i = 0; i < player.lname; ++i) {
		new_pl.myName[i] = player.name[i];
	}
	new_pl.lvl = player.lvl;

	network_client_send(&new_pl);
}

void network_on_client_disconnected() {
	cout << "Client: Disconnected" << endl;
}

void network_on_client_message(uint8_t packet, const void *data) {
	switch (packet) {

  case PacketServerSendCoordinates::ID: {
    auto p = reinterpret_cast<const PacketServerSendCoordinates *>(data);

    if (p->id == player.id) {
      return;
    }

    if (p->x > playerscl[p->id].x) {
      playerscl[p->id].retotate = 0;
    }
    if (p->x < playerscl[p->id].x) {
      playerscl[p->id].retotate = 180;
    }
    if (p->y > playerscl[p->id].y) {
      playerscl[p->id].retotate = 90;
    }
    if (p->y < playerscl[p->id].y) {
      playerscl[p->id].retotate = 270;
    }

    playerscl[p->id].isCreate = true;
    playerscl[p->id].x = p->x;
    playerscl[p->id].y = p->y;
    cout << "true " << p->x << " " << p->y << "\n";


    break;
  }

  case PacketServerSendPlayersInfo::ID: {
		auto p = reinterpret_cast<const PacketServerSendPlayersInfo *>(data);

		if (p->id == player.id) {
      return;
		}

		for (int j = 0; j < p->lname; ++j) {
			playerscl[p->id].name[j] = p->name[j];
		}

		playerscl[p->id].lname = p->lname;
		playerscl[p->id].health = p->health;
		playerscl[p->id].team = p->team;
		playerscl[p->id].isCreate = true;
		cout << "---" << p->id << "\n";

		break;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	case PacketServerSendConnectInfo::ID: {
		auto p = reinterpret_cast<const PacketServerSendConnectInfo *>(data);

		player.id = p->id;
		player.health = p->health;
		player.maxhealth = p->health;
		player.team = p->team;
		player.isCreate = true;

		break;
	}

	case PacketServerRespawnPlayer::ID: {
		auto p = reinterpret_cast<const PacketServerRespawnPlayer *>(data);

		player.x = p->x;
		player.y = p->y;

		break;
	}

	case PacketServerCreateBomb::ID: {
		auto p = reinterpret_cast<const PacketServerCreateBomb *>(data);

		createBomb(p->x, p->y, p->id, p->power);

		break;
	}

	case PacketServerMap::ID: {
		auto p = reinterpret_cast<const PacketServerMap *>(data);

		sizemcl = p->sizem;
		for (int i = 0; i < p->sizem; ++i) {
			for (int j = 0; j < p->sizem; ++j) {
				mapscl[i][j] = p->maps[i][j];
//				cout << p->maps[i][j] << " ";
			}
//			cout << "\n";
		}

		break;
	}
  }
}


int main(int argc, char *argv[]) {
  bool isServer = true;

  if (argc > 1) {
    if (strcmp(argv[1], "server") == 0) {
      isServer = true;
    } else if (strcmp(argv[1], "client") == 0) {
      isServer = false;
    }
  }

  if (isServer) {
    app.setTitle("Server");

    app.setPosition({1366, 0});
    app.setSize({1920 / 2, 1080});
  } else {
    app.setTitle("Client");

    app.setPosition({1366 + 1920 / 2, 0});
    app.setSize({1920 / 2, 1080});
  }

	network_init();

	app.setVerticalSyncEnabled(true);
//	app.setFramerateLimit(20);

	srand(time(0));

	setup();

	if (isServer) {
    start_server();
  }
	network_connect("127.0.0.1", PORT);

	while (app.isOpen()) {
		network_update();
		server_update();
		events();

		controller();
	}

	if (isServer) {
    stop_server();
  }

	//	button1.setSize(Vector2f(10, 10)); (circles)

	//sf::RectangleShape line(sf::Vector2f(150, 5));
	//line.rotate(45); (all) return | - / or \;

	return 0;
}

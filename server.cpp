#include "server.h"
#include "network.h"
#include "packets.h"

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int bomb_count = 0;

struct Player {
    float x = 0, y = 0;
    bool isCreate = false;
    int team = 0, speed = 100, radius = 25, health = 2, maxhealth = 10000, lvl;
    Clock respawntimer, bombtimer;
    string name;
    int lname;
    int maxbombcount = 1, powerboom = 5, rickins = 1;
    bool has_burn_wall = false, has_radio_bomb = false, has_super_bomb = false;
} players[32];

/* select team for players */

struct PlayerSpawn {
    float x, y;
    bool isCreate = false;
    int team = 0;

} spawns[2000];
int spawns_count = 0;

struct Bomb {
    float x = 0, y = 0;
    Clock timerboom;
    int author = 1, powerboom, team = 0;
    int id;
} sbomb;
vector<Bomb *> bombs;

enum class BoomDirection : int {
    CENTER = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
};

struct boom {
    float x = 0, y = 0;
    int team = 0, author, powerboom;
    Clock btimer;
    BoomDirection direction;
} sboom;
vector<boom> booms;



//int *mapm[] = {
//		//      1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
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

int structspawn[3][3]{
    30, 1, 30,
    1, 3, 1,
    30, 1, 30
};
int structspawnsize = 3;


int maps[1024][1024];
int sizem = 64;

void destroy_block(int i, int j, int playerid) {

  if (maps[i][j] < 0) {

    PacketServerPlayerTakeBonus bonus;

    if (maps[i][j] == -1) {
      if (players[playerid].maxbombcount <= 6) {
        ++players[playerid].maxbombcount;
        maps[i][j] = 0;
        bonus.bonus = 1; // bomb count
      }
    }

    if (maps[i][j] == -2) {
      if (players[playerid].powerboom <= 6) {
        ++players[playerid].powerboom;
        maps[i][j] = 0;
        bonus.bonus = 2; // power
      }
    }

    if (maps[i][j] == -3) {
      if (players[playerid].rickins <= 4) {
        ++players[playerid].rickins;
        maps[i][j] = 0;
        bonus.bonus = 3; // rickins
      }
    }

    if (maps[i][j] == -4) {
      if (!players[playerid].has_radio_bomb) {
        players[playerid].has_radio_bomb = true;
        maps[i][j] = 0;
        bonus.bonus = 4; // radio bomb
      }
    }

    if (maps[i][j] == -5) {
      if (!players[playerid].has_burn_wall) {
        players[playerid].has_burn_wall = true;
        maps[i][j] = 0;
        bonus.bonus = 5; // burn wall
      }
    }

    if (maps[i][j] == -6) {
      if (!players[playerid].has_super_bomb) {
        players[playerid].has_super_bomb = true;
        maps[i][j] = 0;
        bonus.bonus = 6; // super bomb
      }
    }

    network_server_send(playerid, &bonus);

  }

  if (maps[i][j] >= 50) {
    int r = rand();

    if (r % 13 == 0) {
      maps[i][j] = -1; // bomb count
    }

    if (r % 17 == 0) {
      maps[i][j] = -2; // power
    }

    if (r % 19 == 0) {
      maps[i][j] = -3; // rickins
    }

    if (r % 27 == 0) {
      maps[i][j] = -4; // radio bomb
    }

    if (r % 41 == 0) {
      maps[i][j] = -5; // burn wall
    }

    if (r % 53 == 0) {
      maps[i][j] = -6; // super bomb
    }

  }


  if (maps[i][j] == 50) {
    maps[i][j] = 0;
  }




  PacketServerDestroyBlock dcoord;
  dcoord.i = i;
  dcoord.j = j;
  dcoord.tile = maps[i][j];
  network_server_broadcast(&dcoord);
}

void createmap() {

  int spawnprobability = 1000;

  for (int i = 0; i < sizem; ++i) {
    for (int j = 0; j < sizem; ++j) {
      if ((i == 0) || (j == 0) || (i == sizem - 1) || j == sizem - 1) {
        maps[i][j] = 25;
        continue;
      }
      maps[i][j] = 0;

      int bl = rand();

      ////////////////////////////////////////////////////////////////// 1 -->

      if ((maps[i - 1][j] == 1) xor (maps[i][j - 1] == 1)) {
        if (bl % 2 == 0) {
          maps[i][j] = 1;
        }
      } else if ((maps[i - 1][j] == 1) && (maps[i][j - 1] == 1)) {
        maps[i][j] = 1;
      } else if (bl % 10 == 0) {
        maps[i][j] = 1;
      }

      /////////////////////////////////////////////////////////////////// 3 -->

      if ((i > structspawnsize) && (j > structspawnsize)) {
        if (rand() % spawnprobability == 0) {
          spawnprobability += 1000;
          spawns[spawns_count].x = (i - 2) * 50;
          spawns[spawns_count].y = (j - 2) * 50;
          spawns[spawns_count].isCreate = true;
          spawns[spawns_count].team = 0;
          ++spawns_count;

          for (int l = i - structspawnsize; l < i; ++l) {
            for (int r = j - structspawnsize; r < j; ++r) {
              maps[l][r] = structspawn[l - i + structspawnsize][r - j + structspawnsize];
            }
          }
        }
      }

      /////////////////////////////////////////////////////////////////// 30-34 -->

      if ((maps[i - 1][j] == 30) xor (maps[i][j - 1] == 30)) {
        if (bl % 3 == 0) {
          maps[i][j] = 30;
        }
      } else if ((maps[i - 1][j] == 30) && (maps[i][j - 1] == 30)) {
        if (bl % 2 == 0) {
          maps[i][j] = 30;
        }
      } else if (bl % 40 == 0) {
        maps[i][j] = 30;
      }

      ////////////////////////////////////////////////////////////////// 50 -->

      if ((maps[i - 1][j] == 50) xor (maps[i][j - 1] == 50)) {
        if (bl % 3 == 0) {
          maps[i][j] = 50;
        }
      } else if ((maps[i - 1][j] == 50) && (maps[i][j - 1] == 50)) {
        if (bl % 2 == 0) {
          maps[i][j] = 50;
        }
      } else if (bl % 4 == 0) {
        maps[i][j] = 50;
      }

      //////////////////////////////////////////////////////////////////////////////


      --spawnprobability;
      spawnprobability = max(spawnprobability, 1);

    }
  }


}


void createBoomUp(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] < 0) {
      destroy_block(i1, j1, 0);
    }
    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1, 0);
    }


    sboom.author = u;
    sboom.x = x;
    sboom.y = y;
    sboom.powerboom = powerboom;
    sboom.team = players[u].team;
    sboom.direction = BoomDirection::UP;
    sboom.btimer.restart();

    booms.push_back(sboom);

    if (powerboom > 0) {
      createBoomUp(u, powerboom - 1, x, y - 50);
    }

}

void createBoomDown(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;


    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] < 0) {
      destroy_block(i1, j1, 0);
    }
    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1, 0);
    }


  sboom.author = u;
  sboom.x = x;
  sboom.y = y;
  sboom.powerboom = powerboom;
  sboom.team = players[u].team;
  sboom.direction = BoomDirection::DOWN;
  sboom.btimer.restart();

  booms.push_back(sboom);

    if (powerboom > 0) {
      createBoomDown(u, powerboom - 1, x, y + 50);
    }

}

void createBoomRight(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;



    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] < 0) {
      destroy_block(i1, j1, 0);
    }
    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1, 0);
    }


  sboom.author = u;
  sboom.x = x;
  sboom.y = y;
  sboom.powerboom = powerboom;
  sboom.team = players[u].team;
  sboom.direction = BoomDirection::RIGHT;
  sboom.btimer.restart();

  booms.push_back(sboom);

    if (powerboom > 0) {
      createBoomRight(u, powerboom - 1, x + 50, y);
    }


}

void createBoomLeft(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;


    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] < 0) {
      destroy_block(i1, j1, 0);
    }
    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1, 0);
    }


  sboom.author = u;
  sboom.x = x;
  sboom.y = y;
  sboom.powerboom = powerboom;
  sboom.team = players[u].team;
  sboom.direction = BoomDirection::LEFT;
  sboom.btimer.restart();

  booms.push_back(sboom);

    if (powerboom > 0) {
      createBoomLeft(u, powerboom - 1, x - 50, y);
    }


}

void createBoomCenter(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

  y += 25;


    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] < 0) {
      destroy_block(i1, j1, 0);
    }
    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1, 0);
    }


    sboom.author = u;
    sboom.x = x;
    sboom.y = y;
    sboom.powerboom = powerboom;
    sboom.team = players[u].team;
    sboom.direction = BoomDirection::CENTER;
    sboom.btimer.restart();

    booms.push_back(sboom);

    if (powerboom > 0) {
      createBoomUp(u, powerboom - 1, x, y - 50);
      createBoomDown(u, powerboom - 1, x, y + 50);
      createBoomRight(u, powerboom - 1, x + 50, y);
      createBoomLeft(u, powerboom - 1, x - 50, y);
    }

}

void createBomb(int u) {


  if (players[u].health > 0 && players[u].maxbombcount > 0) {
    auto bomb = new Bomb();

    bomb->x = floor((players[u].x + 25) / 50) * 50;
    bomb->y = floor((players[u].y + 25) / 50) * 50 - 25;
    bomb->team = players[u].team;
    bomb->timerboom.restart();
    bomb->author = u;
    bomb->powerboom = players[u].powerboom;
    ++bomb_count;
    --players[u].maxbombcount;

    bombs.push_back(bomb);

  }
}

void bombControl() {

//  for (auto bomb_ : bombs) {
//
//
//    if (bomb_.timerboom.getElapsedTime().asSeconds() >= 2.3) {
//      createBoomCenter(bomb_.author, bomb_.powerboom, bomb_.x, bomb_.y);
//
//      ++players[bomb_.author].maxbombcount;
////      delete bomb
//
//      PacketServerActivateBomb bomb1;
//
//      bomb1.power = bomb_.powerboom;
////      bomb1.has_super_bomb = players[bomb_.author].has_super_bomb;
////      bomb1.has_burn_wall = players[bomb_.author].has_burn_wall;
//      bomb1.x = bomb_.x;
//      bomb1.y = bomb_.y;
//      bomb1.team = players[bomb_.author].team;
//
//      network_server_broadcast(&bomb1);
//
//    }
//
//  }

  for (auto it = bombs.cbegin(); it != bombs.cend(); ) {
    auto bomb = *it;

    if (bomb->timerboom.getElapsedTime().asSeconds() >= 2.3) {

      createBoomCenter(bomb->author, bomb->powerboom, bomb->x, bomb->y);

      ++players[bomb->author].maxbombcount;

      it = bombs.erase(it);

      PacketServerActivateBomb bombPacket;

      bombPacket.power = bomb->powerboom;
//      bombPacket.has_super_bomb = players[bomb_.author].has_super_bomb;
//      bombPacket.has_burn_wall = players[bomb_.author].has_burn_wall;
      bombPacket.x = bomb->x;
      bombPacket.y = bomb->y;
      bombPacket.team = players[bomb->author].team;

      network_server_broadcast(&bombPacket);

    } else ++it;
  }

}

//for(auto itr = files.cbegin(); itr != files.cend();) {
//if (exists(*itr)) {
//std::cout << "Found file: " << *itr << "\n";
//itr = files.erase(itr);
//} else
//++itr;
//}

void respawnplayer(int i) {

  int i1 = rand() % spawns_count;

  if (players[i].team == spawns[i1].team) {
    players[i].x = spawns[i1].x;
    players[i].y = spawns[i1].y;

    PacketServerRespawnPlayer newCoord;

    newCoord.x = spawns[i1].x;
    newCoord.y = spawns[i1].y;

    network_server_send(i, &newCoord);
  }
}

void ServerSendCoordinates() {

  PacketServerSendCoordinates players_coord;

  for (int i = 0; i < 32; ++i) {

    if (!players[i].isCreate) {
      continue;
    }

    players_coord.x = players[i].x;
    players_coord.y = players[i].y;
    players_coord.id = i;
    for (int j = 0; j < 32; ++j) {
      network_server_send(j, &players_coord);
    }
  }


}


void network_on_server_added(int i) {
  cout << "Server: Connected #" << i << endl;
}

void network_on_server_removed(int i) {
  cout << "Server: Disconnected #" << i << endl;
}

void network_on_server_message(int i, uint8_t packet, const void *data) {
  switch (packet) {
  case PacketPlayerCreateBomb::ID: {
    auto p = reinterpret_cast<const PacketPlayerCreateBomb *>(data);

    if (players[p->id].health <= 0) {
      return;
    }

    if (players[p->id].maxbombcount <= 0) {
      return;
    }

      if (players[p->id].health > 0 && players[p->id].maxbombcount > 0) {


        PacketServerCreateBomb new_bomb;
        new_bomb.x = floor((players[p->id].x + 25) / 50) * 50;
        new_bomb.y = floor((players[p->id].y + 25) / 50) * 50 - 25;
        new_bomb.power = players[p->id].powerboom;

        createBomb(p->id);

        network_server_broadcast(&new_bomb);


    }


    break;
  }

    ///////////////////////////////////////////////////////////////////////////////////////////

  case PacketPlayerConnect::ID: {
    auto p = reinterpret_cast<const PacketPlayerConnect *>(data);

    PacketServerSendPlayersInfo inform;


    for (int u = 0; u < p->lmyName; ++u) {
      players[i].name[u] = p->myName[u];
      inform.name[u] = p->myName[u];
    }

    players[i].lvl = p->lvl;
    players[i].isCreate = true;

    inform.team = players[i].team;
    inform.health = players[i].maxhealth;
    inform.id = i;
    inform.lname = p->lmyName;
    players[i].lname = p->lmyName;


    PacketServerSendConnectInfo Connect_Info;
    Connect_Info.id = i;
    Connect_Info.team = inform.team;
    Connect_Info.health = inform.health;

    PacketServerMap send_map;

    respawnplayer(i);

    send_map.sizem = sizem;

    for (int u = 0; u < sizem; ++u) {
      for (int j = 0; j < sizem; ++j) {
        send_map.maps[u][j] = maps[u][j];
      }
    }

//		network_server_send(Connect_Info.id, "PacketServerSendConnectInfo", &Connect_Info);
//		network_server_send(Connect_Info.id, "PacketServerMap", &send_map);
    network_server_send(i, &Connect_Info);
    network_server_send(i, &send_map);


    network_server_broadcast(&inform);

    break;
  }

    //////////////////////////////////////////////////////////////////////////////////////////////////

  case PacketPlayerMyCoordinates::ID: {
    auto p = reinterpret_cast<const PacketPlayerMyCoordinates *>(data);
    int j = p->id;

    players[j].x = p->x;
    players[j].y = p->y;

    ServerSendCoordinates();

    if (maps[(int)floor(players[j].x/50)][(int)floor(players[j].y/50)] < 0) {
      destroy_block((int)floor(players[j].x/50), (int)floor(players[j].y/50), j);
    }
    if (maps[(int)floor(players[j].x/50)+1][(int)floor(players[j].y/50)+1] < 0) {
      destroy_block((int)floor(players[j].x/50)+1, (int)floor(players[j].y/50)+1, j);
    }
    if (maps[(int)floor(players[j].x/50)+1][(int)floor(players[j].y/50)] < 0) {
      destroy_block((int)floor(players[j].x/50)+1, (int)floor(players[j].y/50), j);
    }
    if (maps[(int)floor(players[j].x/50)][(int)floor(players[j].y/50)+1] < 0) {
      destroy_block((int)floor(players[j].x/50), (int)floor(players[j].y/50)+1, j);
    }

    break;
  }
  }
}

void start_server() {
  network_start_server(PORT);
  createmap();
}

void stop_server() {
  network_stop_server();
}

void server_update() {
//	ServerSendCoordinates();
  bombControl();
}

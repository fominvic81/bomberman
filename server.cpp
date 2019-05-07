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
    int team = 0, speed = 100, radius = 25, health = 2, maxhealth = 10000, maxbombcount = 1, powerboom = 5, lvl;
    Clock respawntimer, bombtimer;
    string name;
    int lname;
} players[2000];

/* select team for players */

struct PlayerSpawn {
    float x, y;
    bool isCreate = false;
    int team = 0;

} spawns[2000];
int spawns_count = 0;

struct bomb {
    float x = 0, y = 0;
    Clock timerboom;
    int author = 1, powerboom, team = 0;
    bool isCreate = false;
} bombs[2000];

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
    bool isCreate = false;
    Clock btimer;
    BoomDirection direction;
} booms[10000];



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
int sizem = 128;

void destroy_block(int i, int j) {
  if (maps[i][j] == 50) {
    maps[i][j] = 0;
  }


  PacketServerDestroyBlock dcoord;
  dcoord.i = i;
  dcoord.j = j;
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
              maps[l][r] = structspawn[l - i + 3][r - j + 3];
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

  for (int i = 0; i <= 9999; ++i) {
    if (booms[i].isCreate) {
      continue;
    }

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] >= 25) {
      powerboom = 0;
      destroy_block(i1, j1);
    }

    booms[i].author = u;
    booms[i].x = x;
    booms[i].y = y;
    booms[i].isCreate = true;
    booms[i].powerboom = powerboom;
    booms[i].team = players[u].team;
    booms[i].direction = BoomDirection::UP;
    booms[i].btimer.restart();

    if (powerboom > 0) {
      createBoomUp(u, powerboom - 1, x, y - 50);
//			cout << powerboom << "\n";
    }

    break;
  }
}

void createBoomDown(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

  for (int i = 0; i <= 9999; ++i) {
    if (booms[i].isCreate) {
      continue;
    }

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] >= 25) {
//      PacketServerDestroyBlock dcoord;
      powerboom = 0;
//      dcoord.i = i1;
//      dcoord.j = j1;

      destroy_block(i1, j1);
//      network_server_broadcast(&dcoord);
    }

    booms[i].author = u;
    booms[i].x = x;
    booms[i].y = y;
    booms[i].isCreate = true;
    booms[i].powerboom = powerboom;
    booms[i].team = players[u].team;
    booms[i].direction = BoomDirection::DOWN;
    booms[i].btimer.restart();

    if (powerboom > 0) {
      createBoomDown(u, powerboom - 1, x, y + 50);
    }

    break;
  }
}

void createBoomRight(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

  for (int i = 0; i <= 9999; ++i) {
    if (booms[i].isCreate) {
      continue;
    }

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] >= 25) {
//      PacketServerDestroyBlock dcoord;
      powerboom = 0;
//      dcoord.i = i1;
//      dcoord.j = j1;

      destroy_block(i1, j1);
//      network_server_broadcast(&dcoord);
    }

    booms[i].author = u;
    booms[i].x = x;
    booms[i].y = y;
    booms[i].isCreate = true;
    booms[i].powerboom = powerboom;
    booms[i].team = players[u].team;
    booms[i].direction = BoomDirection::RIGHT;
    booms[i].btimer.restart();

    if (powerboom > 0) {
      createBoomRight(u, powerboom - 1, x + 50, y);
    }

    break;
  }
}

void createBoomLeft(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

  for (int i = 0; i <= 9999; ++i) {
    if (booms[i].isCreate) {
      continue;
    }

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] >= 25) {
//      PacketServerDestroyBlock dcoord;
      powerboom = 0;
//      dcoord.i = i1;
//      dcoord.j = j1;

      destroy_block(i1, j1);
//      network_server_broadcast(&dcoord);
    }

    booms[i].author = u;
    booms[i].x = x;
    booms[i].y = y;
    booms[i].isCreate = true;
    booms[i].powerboom = powerboom;
    booms[i].team = players[u].team;
    booms[i].direction = BoomDirection::LEFT;
    booms[i].btimer.restart();

    if (powerboom > 0) {
      createBoomLeft(u, powerboom - 1, x - 50, y);
    }

    break;
  }
}

void createBoomCenter(int u, int powerboom, float x, float y) {
//	RectangleShape boom_;

  y += 25;

  for (int i = 0; i <= 9999; ++i) {
    if (booms[i].isCreate) {
      continue;
    }

    int i1 = (int) floor(x / 50), j1 = (int) floor(y / 50);

    if (maps[i1][j1] >= 25) {
//      PacketServerDestroyBlock dcoord;
      powerboom = 0;
//      dcoord.i = i1;
//      dcoord.j = j1;

      destroy_block(i1, j1);
//      network_server_broadcast(&dcoord);
    }

    booms[i].author = u;
    booms[i].x = x;
    booms[i].y = y;
    booms[i].isCreate = true;
    booms[i].powerboom = powerboom;
    booms[i].team = players[u].team;
    booms[i].direction = BoomDirection::CENTER;
    booms[i].btimer.restart();

    if (powerboom > 0) {
      createBoomUp(u, powerboom - 1, x, y - 50);
      createBoomDown(u, powerboom - 1, x, y + 50);
      createBoomRight(u, powerboom - 1, x + 50, y);
      createBoomLeft(u, powerboom - 1, x - 50, y);
    }

    break;
  }
}

void createBomb(int u) {

  for (int i = 0; i < 1999; ++i) {
    if (!bombs[i].isCreate && players[u].health > 0 && players[u].maxbombcount > 0) {
      bombs[i].x = floor((players[u].x + 25) / 50) * 50;
      bombs[i].y = floor((players[u].y + 25) / 50) * 50 - 25;
      bombs[i].team = players[u].team;
      bombs[i].timerboom.restart();
      bombs[i].author = u;
      bombs[i].isCreate = true;
      bombs[i].powerboom = players[u].powerboom;
      ++bomb_count;
      --players[u].maxbombcount;
//      --players[u].maxbombcount;

//      cout << players[u].maxbombcount << "\n";

      break;
    }

  }
}

void bombControl() {

  for (int i = 0; i <= 1000; ++i) {
    if (!bombs[i].isCreate) {
      continue;
    }

    if (bombs[i].timerboom.getElapsedTime().asSeconds() >= 2.3) {
      createBoomCenter(bombs[i].author, bombs[i].powerboom, bombs[i].x, bombs[i].y);

      ++players[bombs[i].author].maxbombcount;
      bombs[i].isCreate = false;
    }

  }

}

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
//		cout << players[i].x << " " << players[i].y << "\n";
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


//    cout << "HELLO\n";
    if (players[p->id].health <= 0) {
      return;
    }

    if (players[p->id].maxbombcount <= 0) {
      return;
    }

    for (int j = 0; j < 1999; ++j) {
      if (!bombs[j].isCreate && players[p->id].health > 0 && players[p->id].maxbombcount > 0) {


        PacketServerCreateBomb new_bomb;
        new_bomb.x = floor((players[p->id].x + 25) / 50) * 50;
        new_bomb.y = floor((players[p->id].y + 25) / 50) * 50 - 25;
        new_bomb.power = players[p->id].powerboom;
        new_bomb.id = j;
        createBomb(p->id);

//        cout << new_bomb.id << " " << new_bomb.power << " " << new_bomb.x << " " << new_bomb.y << "\n";

        network_server_broadcast(&new_bomb);


        break;
      }

    }


//		network_server_send(i, "pong", &r);
    break;
  }

    ///////////////////////////////////////////////////////////////////////////////////////////

  case PacketPlayerConnect::ID: {
    auto p = reinterpret_cast<const PacketPlayerConnect *>(data);
//		cout << i << "\n";

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

    players[p->id].x = p->x;
    players[p->id].y = p->y;

    ServerSendCoordinates();

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

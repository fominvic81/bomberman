#pragma once

#pragma pack(push, 1)
// Client
struct PacketPlayerConnect {
  static const uint8_t ID = 0;

	char myName[20];
	int lmyName;
	int lvl;
};

struct PacketPlayerMyCoordinates {
  static const uint8_t ID = 1;

	int id;
	float x, y;
};

struct PacketPlayerCreateBomb {
  static const uint8_t ID = 2;

	int id;
};

struct PacketPlayerCreateRadioBomb {
  static const uint8_t ID = 3;

	int id;
};

struct PacketPlayerActivateRadioBomb {
  static const uint8_t ID = 4;

	int id;
};


// Server
struct PacketServerSendCoordinates {
  static const uint8_t ID = 0;

	float x, y;
	int id;
};

struct PacketServerRespawnPlayer {
  static const uint8_t ID = 1;

	float x, y;
};

struct PacketServerSendPlayersInfo {
  static const uint8_t ID = 2;

	int team;
	int id;
	int health;
	char name[20];
	int lname;
};

struct PacketServerSendConnectInfo {
  static const uint8_t ID = 3;

	int team;
	int id;
	int health;
};

struct PacketServerCreateBomb {
  static const uint8_t ID = 4;

	float x, y;
	int power,id;
};

struct PacketServerCreateRadioBomb {
  static const uint8_t ID = 5;

	float x, y;
	int power, id;
};

struct PacketServerActivateRadioBomb {
  static const uint8_t ID = 6;

	int id;
};

struct PacketServerCreateBoom {
  static const uint8_t ID = 7;

  float x, y;
};

struct PacketServerMap {
  static const uint8_t ID = 8;

	int maps[1024][1024];
	int sizem;
};

struct PacketServerDestroyBlock {
    static const uint8_t ID = 9;
    int i, j;
};

#pragma pack(pop)

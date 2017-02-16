#ifndef SETUP_H
#define SETUP_H

#include <allegro.h>
#include <stdlib.h>
#include <time.h>  
#include <fstream>
#include <algorithm>
#include <string>
#include "Global.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"

using namespace std;

class Player;
class Camera;
class Enemy;

class Setup
{
private: 
	int load_counter_x;
	int load_counter_y;
	int map_size_x[10];
	int map_size_y[10];
	char map_file[10][100][100];
	int level;
	int num_of_levels;
	string line;
	
	int enemy_number;

	int exit_x;
	int exit_y;

	int counter;
	int counter_2;

	void SetupPlayer(Player &player);
	void SetupEnemies(Enemy &enemy);
	void LoadMap(const char *filename, int level);

public:

	Setup();
	~Setup();

	enum BlockType
	{
		Empty = '0',
		Wall = '1',
		Exit = '2',
		PlayerStart = '3',
		Enemy1 = '4',
		Enemy2 = '5', 
		Enemy3 = '6',
		RedDoor = 'r',
		RedKey = 'R',
		BlueDoor = 'b',
		BlueKey = 'B',
		OrangeDoor = 'o',
		OrangeKey = 'O',
		HealthPack = 'H',
		Gold = 'G'
	};

	void Init(Player &player, Enemy &enemy);
	void NewDungeon(Player &player, Enemy &enemy);
	void CreateEnvironment(BITMAP *Buffer, BITMAP *Sprites, Camera &camera, Setup &setup);

	bool isSolid(int, int);
	int getExitX();
	int getExitY();
	bool getMapFile(int, int, char);
	int getEnemyNumber();
	int getMapSizeX();
	int getMapSizeY();
	int getLevel();
	int getNumOfLevels();

	void setMapFile(int, int, char);
	void setLevel(int new_level);
	void rectBorder(BITMAP *bmp, int x1, int y1, int x2, int y2, int width, int color);
};
#endif
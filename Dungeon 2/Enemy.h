#ifndef ENEMY_H
#define ENEMY_H

#include <allegro.h>
#include "Global.h"
#include "Setup.h"

class Setup;
class Player;

class Enemy

{
private:
	int counter;
	int counter_2;
	int frame_counter;
	int current_frame_x;
	int current_frame_y;
	int max_bullets;
	int bullet_speed;

	void move(Setup &setup);
	void shoot(Setup &setup, Player &player);
	void lifeCounter(Player &player, Setup &setup);

public:
	Enemy();
	~Enemy();


	struct EnemyInfo
	{
		int x;
		int y;
		int type;
		int direction;
		int speed;
		int health;
		int rof;
		int damage;
		int rof_counter;
		int available_bullet;
		int previous_bullet_direction;
		bool canShoot;
		bool alive;
	};
	EnemyInfo creature[100];

	struct EnemyBullet
	{
		int x;
		int y;
		int direction;
		bool onScreen;
	};
	EnemyBullet bullet[100][25]; //[enemy][bullet]

	void Init();
	void Update(Setup &setup, Player &player);
	void Draw(BITMAP *Buffer, BITMAP *Sprites, Setup &setup);

	int getMaxBullets();

};

#endif

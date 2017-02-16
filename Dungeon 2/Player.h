#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include <string.h>
#include <time.h>  
#include <math.h>
#include "Global.h"
#include "Setup.h"
#include "Enemy.h"

using namespace std;

class Enemy;
class Setup;

class Player
{
private:

	int player_x;
	int player_y;
	int player_grid_x1;
	int player_grid_x2;
	int player_grid_y1;
	int player_grid_y2;
	int player_speed;
	int sprint_speed;
	int walk_speed;
	int player_direction;
	int last_key_direction;
	int key_counter[4];
	int player_health_initial;
	int player_health;
	int player_stamina;
	int player_stamina_max;
	int stamina_counter_minus;
	int stamina_counter_plus;
	int money;
	int bullet_speed;
	int available_bullet;
	int max_bullets;
	int invulnerable_time;
	int invulnerable_counter;
	int current_frame;
	int frame_counter;
	bool isMoving;
	bool isSprinting;
	bool staminaDepleted;
	bool onExit;
	bool throughExit;
	bool playerInvulnerable;
	bool canShoot;
	bool playerHit;
	bool playerShot;
	int counter;
	int ROF_counter;
	int hit_counter;
	int current_hit_frame_x;
	int current_hit_frame_y;
	int current_gun;
	int gun_switch_counter;
	bool canSwitch;
	int max_guns;
	int new_game_state;
	int max_explosions;
	int available_explosion;
	int explosion_size;

	void move(Setup &setup, Enemy &enemy);
	void nextLevel (int exit_x, int exit_y, Setup &setup);
	void lifeCounter(Enemy &enemy, Setup &setup);
	void invulnerability(int value);
	void shoot(Setup &setup, Enemy &enemy);
	void pickup(Setup &setup);
	int lowestNumber(int, int, int, int);

public:
	Player();
	~Player();

	struct PlayerBullet
	{
		int x;
		int y;
		int direction;
		bool onScreen;
	};
	PlayerBullet bullet[100];

	struct GunInfo
	{
		int rate_of_fire;
		int damage;
	};
	GunInfo gun[10];

	struct ExplosionInfo
	{
		int x;
		int y;
		bool onScreen;
		int counter;
	};
	ExplosionInfo explosion[100];

	enum ScreenState
	{
		Title,
		Game,
		LoadGame,
		Options,
		Credits,
		GameOver
	};



	void Init(int game_state);
	void Update(Setup &setup, Enemy &enemy);
	void Draw(BITMAP *Buffer, BITMAP *Sprites, Setup &setup);

	void explode(int x, int y);

	int getPlayerX();
	int getPlayerY();
	int getPlayerHealthInitial();
	int getThroughExit();
	int getPlayerHealth();
	int getPlayerStamina();
	int getCurrentGun();
	int getMaxBullets();
	int getMaxExplosions();
	int getMoney();
	
	void setPlayerX(int);
	void setPlayerY(int);
	void setPlayerHealth(int);
	void setThroughExit(bool);
	void setNewGameState(int);
	int getGameState();


};

#endif
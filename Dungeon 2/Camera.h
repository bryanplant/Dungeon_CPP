#ifndef CAMERA_H
#define CAMERA_H

#include <allegro.h>
#include "Player.h"
#include "Global.h"
#include "Setup.h"

class Player;
class Setup;

class Camera
{
private:
	int camera_x;
	int camera_y;


public:
	Camera();
	~Camera();

	void Init();
	void Update(int x, int y, Setup &setup);
	void Draw(BITMAP *Buffer, BITMAP *Sprites, Player &player, Setup &setup, int game_state);

	void DisplayHUD(BITMAP *Buffer, BITMAP *Sprites, Player &player, Setup &setup, int game_state);

	void setCameraPosition(int x, int y, Setup &setup);

	int getCameraX();
	int getCameraY();
};
#endif
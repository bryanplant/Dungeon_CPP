#include "Camera.h"


Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::Init()
{
	camera_x = 0;
	camera_y = 0;
}
void Camera::Update(int x, int y, Setup &setup)
{
	setCameraPosition(x, y, setup);
}

void Camera::Draw(BITMAP *Buffer, BITMAP *Sprites, Player &player, Setup &setup, int game_state)
{
	DisplayHUD(Buffer, Sprites, player, setup, game_state);
	blit (Buffer, screen, camera_x, camera_y, 0, 0, screen_width, screen_height);
}

void Camera::setCameraPosition(int x, int y, Setup &setup)
{
	camera_x = (x + player_size / 2) - screen_width / 2;
	camera_y = (y + player_size / 2) - screen_height / 2;

	if (camera_x < 0)
		camera_x = 0;
	if (camera_y < 0)
		camera_y = 0;

	if (camera_x > setup.getMapSizeX()*grid_size - screen_width)
		camera_x = setup.getMapSizeX()*grid_size - screen_width;
	if (camera_y > setup.getMapSizeY()*grid_size - screen_height)
		camera_y = setup.getMapSizeY()*grid_size - screen_height;
}

void Camera::DisplayHUD(BITMAP *Buffer, BITMAP *Sprites, Player &player, Setup &setup, int game_state)
{
	if (game_state == 1)
	{
		//Health
		for (int i = 0; i <= (player.getPlayerHealth() - 1); i++)
		{
			masked_blit(Sprites, Buffer, 201, 561, ((camera_x + screen_width / 2) - 60) + (i*30), (camera_y + screen_height) - 70, 21, 21);
		}

		//Stamina
		rectfill(Buffer, (camera_x + (screen_width / 2)) - 60, (camera_y + screen_height) - 30, (camera_x + (screen_width / 2)) + 80, (camera_y + screen_height) - 40, makecol(11, 75, 7));
		rectfill(Buffer, (camera_x + (screen_width / 2)) - 60, (camera_y + screen_height) - 30, ((camera_x + (screen_width / 2)) - 60) + (player.getPlayerStamina()*2), (camera_y + screen_height) - 40, makecol(30, 198, 19)); 

		//gun
		masked_blit(Sprites, Buffer, 100*player.getCurrentGun(), 560, camera_x + 20, camera_y + 20, 100, 50); 

		//money amount
		rectfill(Buffer, camera_x + 690, camera_y + 10, camera_x + 790, camera_y + 35, color_black);
		setup.rectBorder(Buffer, camera_x + 690, camera_y + 10, camera_x + 792, camera_y + 35, 2, color_gold);
		textout(Buffer, font, "Gold: ", camera_x + 700, camera_y + 20, color_white);
		textprintf(Buffer, font, camera_x + 750, camera_y + 20, color_white, "%i", player.getMoney());
	}
}

int Camera::getCameraX()
{
	return camera_x;
}

int Camera::getCameraY()
{
	return camera_y;
}
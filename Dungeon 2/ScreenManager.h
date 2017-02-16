#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <allegro.h>
#include <string>
#include "Player.h"
#include "Camera.h"
#include "Setup.h"
#include "Enemy.h"
#include <fstream>

using namespace std;

class ScreenManager
{
private:
	void UpdateGameScreen(BITMAP *Buffer); 
	void DrawGameScreen(BITMAP *Buffer); 
	void UpdateGameOverScreen();
	void DrawGameOverScreen(BITMAP *Buffer);
	void UpdateTitleScreen();
	void DrawTitleScreen(BITMAP *Buffer);

	int game_state;
	int menu_item;
	bool canSwitch;
	bool done;

	BITMAP *Sprites;

	BITMAP *GameOver_Image;
	BITMAP *TitleScreen_Image;

	Player player;
	Setup setup;
	Camera camera;
	Enemy enemy;	

public:

	enum ScreenState
	{
		Title,
		Game,
		LoadGame,
		Options,
		Credits,
		GameOver
	};

	ScreenManager();
	~ScreenManager();

	void Init();
	void Update(BITMAP *Buffer);
	void Draw(BITMAP *Buffer);

	int getGameState();
	bool getDone();
	void setGameState();
};
#endif

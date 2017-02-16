#include "ScreenManager.h"


ScreenManager::ScreenManager()
{
}


ScreenManager::~ScreenManager()
{
}

void ScreenManager::Init()
{
	game_state = Title;
	menu_item = 0;
	canSwitch = true;
	done = false;

	Sprites = load_bitmap("Sprites.bmp", NULL);

	GameOver_Image = load_bitmap("LossScreen.bmp", NULL);
	TitleScreen_Image = load_bitmap("TitleScreen.bmp", NULL);

	setup.Init(player, enemy);
	player.Init(game_state);
	camera.Init();
	enemy.Init();

}

void ScreenManager::Update(BITMAP *Buffer)
{
	game_state = player.getGameState();

	switch (game_state)
	{
	case Title:
		UpdateTitleScreen();
		break;
	case Game:
		UpdateGameScreen(Buffer);
		break;
	case GameOver:
		UpdateGameOverScreen();
		break;
	}

	if (key[KEY_ESC])
    {
        done = true;
	}
}

void ScreenManager::Draw(BITMAP *Buffer)
{
	switch (game_state)
	{
	case Title:
		DrawTitleScreen(Buffer);
		break;
	case Game:
		DrawGameScreen(Buffer);
		break;
	case GameOver:
		DrawGameOverScreen(Buffer);
		break;
	}
}

void ScreenManager::UpdateTitleScreen()
{
	if (key[KEY_DOWN] && canSwitch)
	{
		menu_item ++;
		if (menu_item > 3)
			menu_item = 0;

		canSwitch = false;
	}
	else if (key[KEY_UP] && canSwitch)
	{
		menu_item --;
		if (menu_item < 0)
			menu_item = 3;

		canSwitch = false;
	}

	if (!key[KEY_DOWN] && !key[KEY_UP])
	{
		canSwitch = true;
	}
	
	if (key[KEY_ENTER])
	{
		if (menu_item == 0)
		{
			game_state = Game;

			player.Init(game_state);
			setup.Init(player, enemy);
		}
		else if (menu_item == 3)
		{
			done = true;
		}
	}
}

void ScreenManager::DrawTitleScreen(BITMAP *Buffer)
{
	blit (TitleScreen_Image, Buffer, 0, 0, 0, 0, screen_width, screen_height);

	if (menu_item == 0)
		setup.rectBorder(Buffer, 230, 89, 570, 184, 8, color_gold);
	else if (menu_item == 1)
		setup.rectBorder(Buffer, 230, 205, 570, 300, 8, color_gold);
	else if (menu_item == 2)
		setup.rectBorder(Buffer, 229, 326, 569, 421, 8, color_gold);
	else if (menu_item == 3)
		setup.rectBorder(Buffer, 227, 440, 567, 535, 8, color_gold);

	camera.Draw(Buffer, Sprites, player, setup, game_state);
}

void ScreenManager::UpdateGameScreen(BITMAP *Buffer)
{
	if (player.getThroughExit())
		setup.NewDungeon(player, enemy);

	player.Update(setup, enemy);
	enemy.Update(setup, player);
	camera.Update(player.getPlayerX(), player.getPlayerY(), setup);
}

void ScreenManager::DrawGameScreen(BITMAP *Buffer)
{
	setup.CreateEnvironment(Buffer, Sprites, camera, setup);
	enemy.Draw(Buffer, Sprites, setup);
	player.Draw(Buffer, Sprites, setup);
	

	camera.Draw(Buffer, Sprites, player, setup, game_state);
}

void ScreenManager::UpdateGameOverScreen()
{
	if (key[KEY_RIGHT] && canSwitch)
	{
		menu_item ++;
		if (menu_item > 1)
			menu_item = 1;

		canSwitch = false;
	}
	else if (key[KEY_LEFT] && canSwitch)
	{
		menu_item --;
		if (menu_item < 0)
			menu_item = 0;

		canSwitch = false;
	}

	if (!key[KEY_RIGHT] && !key[KEY_LEFT])
	{
		canSwitch = true;
	}
	
	if (key[KEY_ENTER])
	{
		if (menu_item == 0)
		{
			game_state = Game;

			player.Init(game_state);
			setup.Init(player, enemy);
		}
		else if (menu_item == 1)
		{
			done = true;
		}
	}
}

void ScreenManager::DrawGameOverScreen(BITMAP *Buffer)
{
	blit (GameOver_Image, Buffer, 0, 0, 0, 0, screen_width, screen_height);
		
	if (menu_item == 0)
	{
		setup.rectBorder(Buffer, 305, 504, 411, 549, 8, color_gold);
	}
	else if (menu_item == 1)
	{
		setup.rectBorder(Buffer, 404, 504, 504, 549, 8, color_gold);
	}

	camera.Draw(Buffer, Sprites, player, setup, game_state);
}

int ScreenManager::getGameState()
{
	return game_state;
}

bool ScreenManager::getDone()
{
	return done;
}
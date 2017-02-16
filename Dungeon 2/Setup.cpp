#include "Setup.h"

Setup::Setup(void)
{
}


Setup::~Setup(void)
{
}

void Setup::Init(Player &player, Enemy &enemy)
{
	load_counter_x = 0;
	load_counter_y = 0;

	num_of_levels = 0;

	LoadMap("Map1.txt", 0);
	LoadMap("Map2.txt", 1);
	LoadMap("Map3.txt", 2);
	LoadMap("Map4.txt", 3);
	LoadMap("Map5.txt", 4);
	LoadMap("Map7.txt", 5);

	level = 0;

	SetupPlayer(player);
	SetupEnemies(enemy);
}

void Setup::NewDungeon(Player &player, Enemy &enemy)
{
	player.setThroughExit(false);
	SetupPlayer(player);
	SetupEnemies(enemy);
}

void Setup::SetupPlayer(Player &player)
{
	for (counter = 0; counter < map_size_x[level]; counter++)
	{
		for (counter_2 = 0; counter_2 < map_size_y[level]; counter_2++)
		{
			if (map_file[level][counter][counter_2] == PlayerStart)
			{
				player.setPlayerX(counter*grid_size);
				player.setPlayerY(counter_2*grid_size);
			}
		}
	}

	player.setThroughExit(false);
}

void Setup::SetupEnemies(Enemy &enemy)
{
	enemy_number = 0;

	for (counter = 0; counter < map_size_x[level]; counter++)
	{
		for (counter_2 = 0; counter_2 < map_size_y[level]; counter_2++)
		{
			if (map_file[level][counter][counter_2] == Enemy1)
			{
				enemy.creature[enemy_number].x = counter*grid_size;
				enemy.creature[enemy_number].y = counter_2*grid_size;
				enemy.creature[enemy_number].type = 1;
				enemy.creature[enemy_number].direction = 1;
				enemy.creature[enemy_number].speed = 2;
				enemy.creature[enemy_number].health = 3;
				enemy.creature[enemy_number].rof = 150 - (rand() %75);
				enemy.creature[enemy_number].rof_counter = 0;
				enemy.creature[enemy_number].damage = 1;
				enemy.creature[enemy_number].canShoot = true;
				enemy.creature[enemy_number].alive = true;

				enemy_number ++;
			}
			
			else if (map_file[level][counter][counter_2] == Enemy2)
			{
				enemy.creature[enemy_number].x = counter*grid_size;
				enemy.creature[enemy_number].y = counter_2*grid_size;
				enemy.creature[enemy_number].type = 2;
				enemy.creature[enemy_number].direction = 1;
				enemy.creature[enemy_number].speed = 2;
				enemy.creature[enemy_number].health = 3;
				enemy.creature[enemy_number].rof = 150 - (rand() %75);
				enemy.creature[enemy_number].rof_counter = 0;
				enemy.creature[enemy_number].damage = 1;
				enemy.creature[enemy_number].alive = true;

				enemy_number ++;
			}
			
			else if (map_file[level][counter][counter_2] == Enemy3)
			{
				enemy.creature[enemy_number].x = counter*grid_size;
				enemy.creature[enemy_number].y = counter_2*grid_size;
				enemy.creature[enemy_number].type = 3;
				enemy.creature[enemy_number].direction = 1;
				enemy.creature[enemy_number].speed = 2;
				enemy.creature[enemy_number].health = 3;
				enemy.creature[enemy_number].rof = 10;
				enemy.creature[enemy_number].rof_counter = 0;
				enemy.creature[enemy_number].damage = 1;
				enemy.creature[enemy_number].previous_bullet_direction = 0;
				enemy.creature[enemy_number].alive = true;

				enemy_number ++;
			}
		}
	}
}

void Setup::CreateEnvironment(BITMAP *Buffer, BITMAP *Sprites, Camera &camera, Setup &setup)
{
	//background
	rectfill(Buffer, 0, 0, setup.getMapSizeX()*grid_size, setup.getMapSizeY()*grid_size, color_brown);

	//boundaries

	for (counter = 0; counter < map_size_x[level]; counter++)
	{
		for (counter_2 = 0; counter_2 < map_size_y[level]; counter_2++)
		{
			if (map_file[level][counter][counter_2] == Wall)
			{
				//walls
				rectfill(Buffer, counter*grid_size, counter_2*grid_size, (counter + 1)*grid_size, (counter_2 + 1)*grid_size, color_black);
				
				//left
				if (!isSolid(counter - 1, counter_2))
					vline(Buffer, counter*grid_size - 1, counter_2*grid_size, (counter_2 + 1)*grid_size - 1, color_green);
				//right
				if (!isSolid(counter + 1, counter_2))
					vline(Buffer, (counter + 1)*grid_size, counter_2*grid_size, (counter_2+1)*grid_size - 1, color_green);
				//top
				if(!isSolid(counter, counter_2 - 1))
					hline(Buffer, counter*grid_size, counter_2*grid_size - 1, (counter + 1)*grid_size - 1, color_green);
				//bottom
				if (!isSolid(counter, counter_2 + 1))
					hline(Buffer, counter*grid_size, (counter_2 + 1)*grid_size, (counter + 1)*grid_size - 1, color_green);
				
			}
			else if (map_file[level][counter][counter_2] == Exit)
			{
				masked_blit(Sprites, Buffer, 400, 320, counter*grid_size, counter_2*grid_size, grid_size, grid_size);
				exit_x = counter;
				exit_y = counter_2;
			}
			else if (map_file[level][counter][counter_2] == HealthPack)
			{
				masked_blit(Sprites, Buffer, 400, 400, counter*grid_size, counter_2*grid_size, grid_size, grid_size);
			}
			else if (map_file[level][counter][counter_2] == Gold)
			{
				masked_blit(Sprites, Buffer, 400, 480, counter*grid_size, counter_2*grid_size, grid_size, grid_size);
			}
			else if (map_file[level][counter][counter_2] == RedKey)
			{
				masked_blit(Sprites, Buffer, 320, 320, counter*grid_size, counter_2*grid_size, grid_size, 80);
			}
			else if (map_file[level][counter][counter_2] == BlueKey)
			{
				masked_blit(Sprites, Buffer, 320, 400, counter*grid_size, counter_2*grid_size, grid_size, 80);
			}
			else if (map_file[level][counter][counter_2] == OrangeKey)
			{
				masked_blit(Sprites, Buffer, 320, 480, counter*grid_size, counter_2*grid_size, grid_size, 80);
			}
			else if (map_file[level][counter][counter_2] == RedDoor)
			{
				if (map_file[level][counter - 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 0, 320, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 - 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 80, 320, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter + 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 160, 320, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 + 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 240, 320, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else
				{
					masked_blit(Sprites, Buffer, 0, 320, counter*grid_size, counter_2*grid_size, 80, 80);
				}
			}
			else if (map_file[level][counter][counter_2] == BlueDoor)
			{
				if (map_file[level][counter - 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 0, 400, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 - 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 80, 400, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter + 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 160, 400, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 + 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 240, 400, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else
				{
					masked_blit(Sprites, Buffer, 0, 400, counter*grid_size, counter_2*grid_size, 80, 80);
				}
			}
			else if (map_file[level][counter][counter_2] == OrangeDoor)
			{
				if (map_file[level][counter - 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 0, 480, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 - 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 80, 480, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter + 1][counter_2] == Wall)
				{
					masked_blit(Sprites, Buffer, 160, 480, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else if (map_file[level][counter][counter_2 + 1] == Wall)
				{
					masked_blit(Sprites, Buffer, 240, 480, counter*grid_size, counter_2*grid_size, 80, 80);
				}
				else 
				{
					masked_blit(Sprites, Buffer, 0, 480, counter*grid_size, counter_2*grid_size, 80, 80);
				}
			}
		}
	}
		textprintf(Buffer, font, 0, 90, makecol(255,255,255), "%i", map_size_x[0]);
		textprintf(Buffer, font, 0, 100, makecol(255, 255, 255), "%i", map_size_y[0]);
}

void Setup::LoadMap(const char *filename, int level)
{
	ifstream openfile(filename);

	if (openfile.is_open())
	{
		getline(openfile, line);
		line.erase(remove(line.begin(), line.end(), ' ' ), line.end() );
		map_size_x[level] = line.length();
		openfile.seekg(0, ios::beg);
		map_size_y[level] = 0;
		while(!openfile.eof())
		{
			openfile >> map_file[level][load_counter_x][load_counter_y];
			load_counter_x ++;
			if (load_counter_x >= map_size_x[level])
			{
				load_counter_x = 0;
				load_counter_y ++;
				map_size_y[level] ++;
			}
		}
		load_counter_x = load_counter_y = 0;	
		num_of_levels ++;
	}
	else
	{
		allegro_message("Could not load the specified map file");
	}
}

bool Setup::isSolid(int x, int y)
{
	if (map_file[level][x][y] == Wall || map_file[level][x][y] == RedDoor || map_file[level][x][y] == BlueDoor  || map_file[level][x][y] == OrangeDoor)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Setup::getExitX()
{
	return exit_x;
}

int Setup::getExitY()
{
	return exit_y;
}

bool Setup::getMapFile(int x, int y, char value)
{
	if (map_file[level][x][y] == value)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

int Setup::getEnemyNumber()
{
	return enemy_number;
}

int Setup::getMapSizeX()
{
	return map_size_x[0];
}

int Setup::getMapSizeY()
{
	return map_size_y[level];
}

int Setup::getLevel()
{
	return level;
}

int Setup::getNumOfLevels()
{
	return num_of_levels;
}

void Setup::setLevel(int new_level)
{
	level = new_level;
}

void Setup::setMapFile(int x, int y, char value)
{
	map_file[level][x][y] = value;
}

void Setup::rectBorder(BITMAP *bmp, int x1, int y1, int x2, int y2, int width, int color)
{
	rectfill(bmp, x1, y1, x2, y1 + width, color);
	rectfill(bmp, x1, y2, x2, y2 - width, color);
	rectfill(bmp, x1, y1, x1 + width, y2, color);
	rectfill(bmp, x2 - width, y1, x2, y2, color); 
}
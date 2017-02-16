#include "Player.h"


Player::Player(void)
{
}


Player::~Player(void)
{
}

void Player::Init(int game_state)
{
	walk_speed = 4;
	sprint_speed = 6;
	player_speed = walk_speed;
	player_direction = 2;
	last_key_direction = 2;
	player_health_initial = 5;
	player_health = player_health_initial;
	player_stamina_max = 70;
	player_stamina = player_stamina_max;
	stamina_counter_minus = 0;
	stamina_counter_plus = 0;
	money = 0;
	invulnerable_counter = 100;
	playerInvulnerable = true;
	isMoving = false;
	isSprinting = false;
	staminaDepleted = false;
	onExit = false;
	throughExit = false;
	canShoot = true;
	playerHit = false;
	playerShot = false;
	current_frame = 0;
	frame_counter = 0;
	counter = 0;
	ROF_counter = 0;
	hit_counter = 0;
	current_hit_frame_x = 0;
	current_hit_frame_y = 0;
	max_bullets = 100;
	bullet_speed = 10;
	available_bullet = 0;
	current_gun = 0;
	gun_switch_counter = 0;
	canSwitch = true;
	max_guns = 1;
	new_game_state = game_state;
	available_explosion = 0;
	max_explosions = 100;
	explosion_size = 100;

	gun[0].rate_of_fire = 100, gun[0].damage = 3;
	gun[1].rate_of_fire = 10, gun[1].damage = 1;

	for (int i = 0; i < 4; i++)
	{
		key_counter[i] = 0;
	}
}

void Player::Update(Setup &setup, Enemy &enemy)
{
	player_grid_x1 = player_x / grid_size;
	player_grid_y1 = player_y / grid_size;
	player_grid_x2 = ((player_x - 1) + player_size) / grid_size;
	player_grid_y2 = ((player_y - 1) + player_size) / grid_size;

	move(setup, enemy);
	shoot(setup, enemy);
	nextLevel(setup.getExitX(), setup.getExitY(), setup);
	lifeCounter(enemy, setup);
	pickup(setup);
}

void Player::Draw(BITMAP *Buffer, BITMAP *Sprites, Setup &setup)
{
	//bullets
	for (counter = 0; counter <= max_bullets; counter ++)
	{
		if (bullet[counter].onScreen == true)
			circlefill(Buffer, bullet[counter].x, bullet[counter].y, bullet_size, color_black);
	}

	masked_blit(Sprites, Buffer, current_frame * player_size, player_direction * player_size, player_x, player_y, player_size, player_size);

	textprintf(Buffer, font, 0, 0, makecol(255,255,255), "%i", key_counter[0]);
	textprintf(Buffer, font, 0, 10, makecol(255,255,255), "%i", key_counter[1]);
	textprintf(Buffer, font, 0, 20, makecol(255,255,255), "%i", key_counter[2]);
	textprintf(Buffer, font, 0, 30, makecol(255,255,255), "%i", key_counter[3]);

	if (playerHit || playerShot)
	{
		if (playerHit)
		{
			masked_blit(Sprites, Buffer, current_hit_frame_x*100 + 324, current_hit_frame_y*100 + 561, player_x, player_y, 100, 100);
		}

		else if (playerShot)
		{
			masked_blit(Sprites, Buffer, current_hit_frame_x*40 + 203, 662, player_x + (player_size / 4), player_y + (player_size / 4), 40, 40);
		}
		
		hit_counter ++;
		if (hit_counter == 3)
		{
			current_hit_frame_x = 1;
		}
		else if (hit_counter == 6)
		{
			current_hit_frame_x = 2;
		}
		else if (hit_counter == 9)
		{
			current_hit_frame_x = 3;
		}
		else if (hit_counter == 12)
		{
			current_hit_frame_x = 4;
		}
		else if (hit_counter >= 15)
		{
			playerHit = false;
			playerShot = false;
			hit_counter = 0;
			current_hit_frame_x = 0;
			current_hit_frame_y = rand() %4;
		}
	}

	//Explosion
	for (int i = 0; i < max_explosions; i ++)
	{
		if (explosion[i].onScreen)
		{
			if (explosion[i].counter < 15)
				masked_blit(Sprites, Buffer, 223, 561, explosion[i].x - 70, explosion[i].y - 60, explosion_size, explosion_size);
			if (explosion[i].counter > 5 && explosion[i].counter < 30)
				masked_blit(Sprites, Buffer, 223, 561, explosion[i].x - 10, explosion[i].y - 50, explosion_size, explosion_size);
			if (explosion[i].counter > 20 && explosion[i].counter < 35)
				masked_blit(Sprites, Buffer, 223, 561, explosion[i].x - 50, explosion[i].y - 15, explosion_size, explosion_size);
			if (explosion[i].counter > 40)
			{
				explosion[i].counter = 0;
				explosion[i].onScreen = false;
			}

			explosion[i].counter ++;
		}
	}

	if (onExit)
		textout_centre_ex(Buffer, font, "*Enter*", (setup.getExitX()*grid_size) + (exit_size / 2), (setup.getExitY()*grid_size) + (exit_size / 2), makecol(255, 255, 255), -1); 
}

void Player::move(Setup &setup, Enemy &enemy)
{
	isMoving = true;

	if (key[KEY_LSHIFT] && isMoving && player_stamina > 0 && !staminaDepleted)
	{
		player_speed = sprint_speed;
		isSprinting = true;
	}
	else
	{
		isSprinting = false;
		player_speed = walk_speed;
	}

	//Move & Block Collision
	if (key[KEY_D])
		key_counter[0] ++;
	else
		key_counter[0] = 0;
	if (key[KEY_S])
		key_counter[1] ++;
	else
		key_counter[1] = 0;
	if (key[KEY_A])
		key_counter[2] ++;
	else
		key_counter[2] = 0;
	if (key[KEY_W])
		key_counter[3] ++;
	else
		key_counter[3] = 0;

	last_key_direction = lowestNumber(key_counter[0], key_counter[1], key_counter[2], key_counter[3]);

	if (!key[KEY_D] && !key[KEY_S] && !key[KEY_A] && !key[KEY_W])
		last_key_direction = -1;

	switch(last_key_direction)
	{
	case 0:
		if (setup.isSolid(player_grid_x1 + 1, player_grid_y1) || setup.isSolid(player_grid_x1 + 1, player_grid_y2))
		{
			player_x = player_grid_x1*grid_size;
			isMoving = false;
		} 
		else
		{
		
			player_x += player_speed;		
		}
		player_direction = 0;
		break;

	case 1:
		if (setup.isSolid(player_grid_x1, player_grid_y1 + 1) || setup.isSolid(player_grid_x2, player_grid_y1 + 1))
		{
			player_y = player_grid_y1*grid_size;
			isMoving = false;
		} 
		else
		{
				player_y += player_speed;
		}		
		player_direction = 1;
		break;
		
	case 2:
		if (setup.isSolid(player_grid_x2 - 1, player_grid_y1) || setup.isSolid(player_grid_x2 - 1, player_grid_y2))
		{
			player_x = player_grid_x2*grid_size;
			isMoving = false;
		} 
		else
		{
			player_x -= player_speed;		
		}
		player_direction = 2;	
		break;
		
	case 3:
		if (setup.isSolid(player_grid_x1, player_grid_y2 - 1) || setup.isSolid(player_grid_x2, player_grid_y2 - 1))
		{
			player_y = player_grid_y2*grid_size;
			isMoving = false;
		} 
		else
		{
			player_y -= player_speed;
		}	
		player_direction = 3;
		break;

	default:
		isMoving = false;
		break;
	}

	//Stamina
	if (isMoving && isSprinting && !staminaDepleted)
	{
		stamina_counter_plus = 0;

		if (stamina_counter_minus >= 10)
		{
			player_stamina -= 1;
			stamina_counter_minus = 0;

			if (player_stamina <= 0)
			{
				player_stamina = 0;
				staminaDepleted = true;
			}
		}
		stamina_counter_minus ++;
	}
	else 
	{
		stamina_counter_minus = 0;

		if (stamina_counter_plus >= 15)
		{
			player_stamina += 1;
			stamina_counter_plus = 0;

			if (player_stamina > player_stamina_max)
				player_stamina = player_stamina_max;

			if (player_stamina > 20)
				staminaDepleted = false;
		}
		stamina_counter_plus ++;
	}


	//Animation
	if (isMoving)
	{
		if (!isSprinting)
		{
			frame_counter++;
			if (frame_counter == 1)
				current_frame = 1;
			else if (frame_counter == 25)
				current_frame = 0;
			else if (frame_counter == 50)
				current_frame = 2;
			else if (frame_counter == 75)
				current_frame = 0;
			else if (frame_counter >= 100)
				frame_counter = 0;
		}

		else if (isSprinting)
		{
			frame_counter += 2;
			if (frame_counter == 2)
				current_frame = 1;
			else if (frame_counter == 30)
				current_frame = 0;
			else if (frame_counter == 58)
				current_frame = 2;
			else if (frame_counter == 86)
				current_frame = 0;
			else if (frame_counter >= 100)
				frame_counter = 0;
		}
	}
	else
	{
		frame_counter = 0;
		current_frame = 0;
	}
}

void Player::nextLevel(int exit_x, int exit_y, Setup &setup)
{
	if (player_grid_x2 < exit_x || player_grid_y2 < exit_y || player_grid_x1 > exit_x || player_grid_y1 > exit_y)
	{
		onExit = false;
	}
	else
	{		
		onExit = true;

		if (key[KEY_ENTER])
		{
			throughExit = true;
			setup.setLevel(setup.getLevel() + 1);

			if (setup.getLevel() == setup.getNumOfLevels())
				setup.setLevel(0);
		}
		else
			throughExit = false;
	}
}

void Player::lifeCounter(Enemy &enemy, Setup &setup)
{ 
	for (int i = 0; i < setup.getEnemyNumber(); i ++)
	{
		if (enemy.creature[i].alive == true)
		{
			if (player_x + player_size <= enemy.creature[i].x || player_x >= enemy.creature[i].x + enemy_size || player_y + player_size <= enemy.creature[i].y || player_y >= enemy.creature[i].y + enemy_size)
			{

			}
			else if (!playerInvulnerable && enemy.creature[i].damage >= 1)
			{
				player_health -= enemy.creature[i].damage;
				playerHit = true;
				invulnerability(150);
			}
		}

		for (int j = 0; j < enemy.getMaxBullets(); j ++)
		{
			if (enemy.bullet[i][j].onScreen)
			{
				if ((enemy.bullet[i][j].x + bullet_size) < player_x || (enemy.bullet[i][j].y + bullet_size) < player_y || enemy.bullet[i][j].x > (player_x + player_size) || enemy.bullet[i][j].y > (player_y + player_size))
				{
					//no collison
				}
				else if (!playerInvulnerable)
				{
					player_health -= enemy.creature[i].damage;
					enemy.bullet[i][j].onScreen = false;
					playerShot = true;
					invulnerability(150);
				}
			}
		}
	}

	if (player_health <= 0)
	{
		player_x = 0;
		player_y = 0;
		new_game_state = GameOver;
	}

	if (playerInvulnerable)
	{
		if (invulnerable_counter >= invulnerable_time)
			playerInvulnerable = false;
		invulnerable_counter++;
	}
}

void Player::invulnerability(int value)
{
	invulnerable_time = value;
	playerInvulnerable = true;
	invulnerable_counter = 0;
}

void Player::shoot(Setup &setup, Enemy &enemy)
{
	//gun switch
	if (canSwitch == false)
	{
		gun_switch_counter ++;

		if (gun_switch_counter >= 40)
		{
			canSwitch = true;
			gun_switch_counter = 0;
		}
	}

	if (canSwitch == true)
	{
		if (key[KEY_Q])
		{
			current_gun ++;
			canSwitch = false;
			if (current_gun > max_guns)
				current_gun = 0;
		}
	}


	if (canShoot)
	{
		for (counter = 0; bullet[counter].onScreen == true && counter < max_bullets; counter ++)
		{
			//not available
		}
		available_bullet = counter;
		
		if (available_bullet > max_bullets)
			available_bullet = max_bullets;

		if (key[KEY_LEFT] || key[KEY_RIGHT] || key[KEY_UP] || key[KEY_DOWN])
		{
			bullet[available_bullet].x = player_x + (player_size / 2) - (bullet_size / 2);
			bullet[available_bullet].y = player_y + (player_size / 2) - (bullet_size / 2);
			bullet[available_bullet].onScreen = true;
			canShoot = false;

			if (key[KEY_RIGHT])
			{
				bullet[available_bullet].direction = 0;
			}
			if (key[KEY_DOWN])
			{
				bullet[available_bullet].direction = 1; 
			}
			else if (key[KEY_LEFT])
			{
				bullet[available_bullet].direction = 2;
			}
			else if (key[KEY_UP])
			{
				bullet[available_bullet].direction = 3;
			}
		}
	}
	else
	{
		if (ROF_counter >= gun[current_gun].rate_of_fire)
		{
			canShoot = true;
			ROF_counter = 0;
		}
		else
		{
			ROF_counter ++;
		}
	}

	for (counter = 0; counter < max_bullets; counter ++)
	{
		if (bullet[counter].onScreen == true)
		{
			//bullet movement
			if (bullet[counter].direction == 0)
			{
				bullet[counter].x += bullet_speed;
			}
			else if (bullet[counter].direction == 1)
				bullet[counter].y += bullet_speed;
			else if (bullet[counter].direction == 2)
				bullet[counter].x -= bullet_speed;
			else if (bullet[counter].direction == 3)
				bullet[counter].y -= bullet_speed;
		
			//block collision
			if (setup.isSolid(bullet[counter].x / grid_size, bullet[counter].y / grid_size) || setup.isSolid((bullet[counter].x + bullet_size) / grid_size, bullet[counter].y / grid_size) ||setup.isSolid(bullet[counter].x / grid_size, (bullet[counter].y + bullet_size) / grid_size) || setup.isSolid((bullet[counter].x + bullet_size) / grid_size, (bullet[counter].y + bullet_size) / grid_size))
			{
				bullet[counter].onScreen = false;
				//explode(bullet[counter].x, bullet[counter].y);
			}
		}
	}



}

void Player::pickup(Setup &setup)
{
	if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.RedKey) || setup.getMapFile(player_grid_x2, player_grid_y1, setup.RedKey) || setup.getMapFile(player_grid_x1, player_grid_y2, setup.RedKey)  || setup.getMapFile(player_grid_x2, player_grid_y2, setup.RedKey))
	{
		for(int i = 0; i < setup.getMapSizeX(); i ++)
		{
			for (int j = 0; j < setup.getMapSizeY(); j ++)
			{
				if (setup.getMapFile(i, j, setup.RedKey) || setup.getMapFile(i, j, setup.RedDoor))
				{
					setup.setMapFile(i, j, setup.Empty);
				}  
			}
		}
	}
	if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.BlueKey) || setup.getMapFile(player_grid_x2, player_grid_y1, setup.BlueKey) || setup.getMapFile(player_grid_x1, player_grid_y2, setup.BlueKey)  || setup.getMapFile(player_grid_x2, player_grid_y2, setup.BlueKey))
	{
		for(int i = 0; i < setup.getMapSizeX(); i ++)
		{
			for (int j = 0; j < setup.getMapSizeY(); j ++)
			{
				if (setup.getMapFile(i, j, setup.BlueKey) || setup.getMapFile(i, j, setup.BlueDoor))
				{
					setup.setMapFile(i, j, setup.Empty);
				}  
			}
		}
	}
	if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.OrangeKey) || setup.getMapFile(player_grid_x2, player_grid_y1, setup.OrangeKey) || setup.getMapFile(player_grid_x1, player_grid_y2, setup.OrangeKey)  || setup.getMapFile(player_grid_x2, player_grid_y2, setup.OrangeKey))
	{
		for(int i = 0; i < setup.getMapSizeX(); i ++)
		{
			for (int j = 0; j < setup.getMapSizeY(); j ++)
			{
				if (setup.getMapFile(i, j, setup.OrangeKey) || setup.getMapFile(i, j, setup.OrangeDoor))
				{
					setup.setMapFile(i, j, setup.Empty);
				}  
			}
		}
	}
	if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.HealthPack) || setup.getMapFile(player_grid_x2, player_grid_y1, setup.HealthPack) || setup.getMapFile(player_grid_x1, player_grid_y2, setup.HealthPack)  || setup.getMapFile(player_grid_x2, player_grid_y2, setup.HealthPack))
	{
		if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.HealthPack))
		{
			setup.setMapFile(player_grid_x1, player_grid_y1, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x2, player_grid_y1, setup.HealthPack))
		{
			setup.setMapFile(player_grid_x2, player_grid_y1, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x1, player_grid_y2, setup.HealthPack))
		{
			setup.setMapFile(player_grid_x1, player_grid_y2, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x2, player_grid_y2, setup.HealthPack))
		{
			setup.setMapFile(player_grid_x2, player_grid_y2, setup.Empty);
		}

		player_health += 2;
		if (player_health > 5)
		{
			player_health = 5;
		}
	}
	
	if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.Gold) || setup.getMapFile(player_grid_x2, player_grid_y1, setup.Gold) || setup.getMapFile(player_grid_x1, player_grid_y2, setup.Gold)  || setup.getMapFile(player_grid_x2, player_grid_y2, setup.Gold))
	{
		if (setup.getMapFile(player_grid_x1, player_grid_y1, setup.Gold))
		{
			setup.setMapFile(player_grid_x1, player_grid_y1, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x2, player_grid_y1, setup.Gold))
		{
			setup.setMapFile(player_grid_x2, player_grid_y1, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x1, player_grid_y2, setup.Gold))
		{
			setup.setMapFile(player_grid_x1, player_grid_y2, setup.Empty);
		}
		else if (setup.getMapFile(player_grid_x2, player_grid_y2, setup.Gold))
		{
			setup.setMapFile(player_grid_x2, player_grid_y2, setup.Empty);
		}

	money += 100;
	}
}

void Player::explode(int x, int y)
{
	for (counter = 0; explosion[counter].onScreen == true && counter < max_explosions; counter ++)
	{
		//not available
	}	
	available_explosion = counter;

	explosion[available_explosion].x = x;
	explosion[available_explosion].y = y;
	explosion[available_explosion].onScreen = true;
	explosion[available_explosion].counter = 0;
}

int Player::lowestNumber(int a, int b, int c, int d)
{
	int small = 0;

	if (a > 0)
		small = a;
	else if (b > 0)
		small = b;
	else if (c > 0)
		small = c;
	else if (d > 0)
		small = d;
	
	if (b < small && b > 0)
		small = b;
	if (c < small && c > 0)
		small = c;
	if (d < small && d > 0)
		small = d;
	
	if (small == a)
		return 0;
	if (small == b)
		return 1;
	if (small == c)
		return 2;
	if (small == d)
		return 3;
}

int Player::getPlayerX()
{
	return player_x;
}

int Player::getPlayerY()
{
	return player_y;
}

int Player::getThroughExit()
{
	return throughExit;
}

int Player::getPlayerHealth()
{
	return player_health;
}

int Player::getPlayerHealthInitial()
{
	return player_health_initial;
}

int Player::getPlayerStamina()
{
	return player_stamina;
}

int Player::getCurrentGun()
{
	return current_gun;
}

int Player::getMaxBullets()
{
	return max_bullets;
}

int Player::getMaxExplosions()
{
	return max_explosions;
}

void Player::setPlayerX(int value)
{
	player_x = value;
}

void Player::setPlayerY(int value)
{
	player_y = value;
}

void Player::setPlayerHealth(int value)
{
	player_health = value;
}

void Player::setThroughExit(bool value)
{
	throughExit = value;
}

void Player::setNewGameState(int game_state)
{
	new_game_state = game_state;
}

int Player::getGameState()
{
	return new_game_state;
}

int Player::getMoney()
{
	return money;
}
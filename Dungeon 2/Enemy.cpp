#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Init()
{
	current_frame_x = 0;
	current_frame_y = 0;
	frame_counter = 0;
	max_bullets = 20;
	bullet_speed = 5;
}

void Enemy::Update(Setup &setup, Player &player)
{
	move(setup);	
	shoot(setup, player);
	lifeCounter(player, setup);
}

void Enemy::Draw(BITMAP *Buffer, BITMAP *Sprites, Setup &setup)
{
	//bullets
	for (int i = 0; i < setup.getEnemyNumber(); i ++)
	{
		for (int j = 0; j < max_bullets; j ++)
		{
			if (bullet[i][j].onScreen)
				circlefill(Buffer, bullet[i][j].x, bullet[i][j].y, bullet_size, color_black);
		}
	}

	//enemies
	for (counter = 0; counter < setup.getEnemyNumber(); counter++) 
	{
		if (creature[counter].type == 1)
		{
			if (creature[counter].direction == 1)
				current_frame_y = 0;
			else if (creature[counter].direction == -1)
				current_frame_y = enemy_size;
		}
		else if (creature[counter].type == 2)
		{
			if (creature[counter].direction == 1)
				current_frame_y = enemy_size*2;
			else if (creature[counter].direction == -1)
				current_frame_y = enemy_size*3;
		}

		if (creature[counter].alive == true)
		{
			if (creature[counter].type == 1 || creature[counter].type == 2)
			{
				masked_blit(Sprites, Buffer, current_frame_x*enemy_size + 240, current_frame_y, creature[counter].x, creature[counter].y, enemy_size, enemy_size);
			}

			else if (creature[counter].type == 3)
			{
				masked_blit(Sprites, Buffer, 480, 0, creature[counter].x, creature[counter].y, 80, 80);
			}
		}
	}
}

void Enemy::move(Setup &setup)
{
	for (counter = 0; counter < setup.getEnemyNumber(); counter ++)
	{
		if (creature[counter].type == 1)
		{
			if (creature[counter].direction == 1 && setup.isSolid((creature[counter].x / grid_size) + 1, (creature[counter].y / grid_size)))
				creature[counter].direction *= -1;
			else if (creature[counter].direction == -1 && setup.isSolid((creature[counter].x / grid_size), (creature[counter].y / grid_size)))
				creature[counter].direction *= -1;
			
			creature[counter].x += (creature[counter].direction * creature[counter].speed);
		}
		
		else if (creature[counter].type == 2)
		{
			if (creature[counter].direction == 1 && setup.isSolid((creature[counter].x / grid_size), (creature[counter].y / grid_size) + 1))
				creature[counter].direction *= -1;
			else if (creature[counter].direction == -1 && setup.isSolid((creature[counter].x / grid_size), (creature[counter].y / grid_size)))
				creature[counter].direction *= -1;
			
			creature[counter].y += (creature[counter].direction * creature[counter].speed);
		}
	}
	
	//animation
	if (frame_counter == 1)
		current_frame_x = 1;
	else if (frame_counter == 25)
		current_frame_x = 0;
	else if (frame_counter == 50)
		current_frame_x = 2;
	else if (frame_counter == 75)
		current_frame_x = 0;
	else if (frame_counter >= 100)
		frame_counter = 0;
	frame_counter++;
}

void Enemy::shoot(Setup &setup, Player &player)
{
	for (int i = 0; i < setup.getEnemyNumber(); i ++)
	{
		if (creature[i].alive)
		{
			if (creature[i].canShoot)
			{
				for (counter = 0; bullet[i][counter].onScreen == true; counter ++)
				{
					//unavailable bullets
				}
				creature[i].available_bullet = counter;
		
				if (creature[i].available_bullet > max_bullets)
					creature[i].available_bullet = max_bullets;

				/*if (creature[i].available_bullet > max_bullets)
					max_bullets = creature[i].available_bullet;*/

				bullet[i][creature[i].available_bullet].x = creature[i].x + (enemy_size / 2) - (bullet_size / 2);
				bullet[i][creature[i].available_bullet].y = creature[i].y + (enemy_size / 2) - (bullet_size / 2);
				bullet[i][creature[i].available_bullet].onScreen = true;
				creature[i].canShoot = false;

					if (creature[i].type == 1)
					{
						if (creature[i].direction == 1)
							bullet[i][counter].direction = 0;
						else if (creature[i].direction == -1)
							bullet[i][counter].direction = 4;
					}
					else if (creature[i].type == 2)
					{
						if (creature[i].direction == 1)
							bullet[i][counter].direction = 2;
						else if (creature[i].direction == -1)
							bullet[i][counter].direction = 6;
					}
					else if (creature[i].type == 3)
					{
						bullet[i][counter].direction = creature[i].previous_bullet_direction + 1;
						creature[i].previous_bullet_direction ++;

						if (bullet[i][counter].direction > 7)
						{
							bullet[i][counter].direction = 0;
							creature[i].previous_bullet_direction = 0;
						}
					}
					
				}
				else
				{
					if (creature[i].rof_counter >= creature[i].rof)
					{
						creature[i].canShoot = true;
						creature[i].rof_counter = 0;
					}
					else
					{
						creature[i].rof_counter ++;
					}
				}
		}
		

		for (int j = 0; j < max_bullets; j ++)
		{
			if (bullet[i][j].onScreen == true)
			{
				//bullet movement
				if (bullet[i][j].direction == 0)
					bullet[i][j].x += bullet_speed;
				else if (bullet[i][j].direction == 2)
					bullet[i][j].y += bullet_speed;
				else if (bullet[i][j].direction == 4)
					bullet[i][j].x -= bullet_speed;
				else if (bullet[i][j].direction == 6)
					bullet[i][j].y -= bullet_speed;
				else if (bullet[i][j].direction == 1)
				{
					bullet[i][j].x += (sqrt(2.0) / 2)*bullet_speed;
					bullet[i][j].y += (sqrt(2.0) / 2)*bullet_speed;
				}
				else if (bullet[i][j].direction == 3)
				{
					bullet[i][j].x -= (sqrt(2.0) / 2)*bullet_speed;
					bullet[i][j].y += (sqrt(2.0) / 2)*bullet_speed;
				}
				else if (bullet[i][j].direction == 5)
				{
					bullet[i][j].x -= (sqrt(2.0) / 2)*bullet_speed;
					bullet[i][j].y -= (sqrt(2.0) / 2)*bullet_speed;
				}
				else if (bullet[i][j].direction == 7)
				{
					bullet[i][j].x += (sqrt(2.0) / 2)*bullet_speed;
					bullet[i][j].y -= (sqrt(2.0) / 2)*bullet_speed;
				}

		
				//block collision
				if (setup.isSolid(bullet[i][j].x / grid_size, bullet[i][j].y / grid_size) || setup.isSolid((bullet[i][j].x + bullet_size) / grid_size, bullet[i][j].y / grid_size) ||setup.isSolid(bullet[i][j].x / grid_size, (bullet[i][j].y + bullet_size) / grid_size) || setup.isSolid((bullet[i][j].x + bullet_size) / grid_size, (bullet[i][j].y + bullet_size) / grid_size))
				{
					bullet[i][j].onScreen = false;
				}				
			}
		}
	}
}

void Enemy::lifeCounter(Player &player, Setup &setup)
{
	for (int i = 0; i <= player.getMaxBullets(); i ++)
	{
		if (player.bullet[i].onScreen)
		{
			for (int j = 0; j < setup.getEnemyNumber(); j ++)
			{
				if (creature[j].alive)
				{
					if ((player.bullet[i].x + bullet_size) < creature[j].x || (player.bullet[i].y + bullet_size) < creature[j].y || player.bullet[i].x > (creature[j].x + enemy_size) || player.bullet[i].y > (creature[j].y + enemy_size))
					{
						//no collison
					}
					else
					{
						creature[j].health -= player.gun[player.getCurrentGun()].damage;
						player.bullet[i].onScreen = false;

						if (creature[j].health <= 0)
						{
							creature[j].alive = false;
							player.explode(creature[j].x + (grid_size / 2), creature[j].y + (grid_size / 2));
							if (setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Empty) || setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Enemy1) || setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Enemy2) || setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Enemy3) || setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.PlayerStart))
							{
								setup.setMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Gold);
							}
						}


					}
				}
			}
		}
	}
	for (int i = 0; i <= player.getMaxExplosions(); i ++)
	{
		if (player.explosion[i].onScreen)
		{
			for (int j = 0; j < setup.getEnemyNumber(); j ++)
			{
				if (creature[j].alive)
				{
					if ((player.explosion[i].x + 90) < creature[j].x || (player.explosion[i].y + 85) < creature[j].y || (player.explosion[i].x - 70) > (creature[j].x + enemy_size) || (player.explosion[i].y - 60) > (creature[j].y + enemy_size))
					{
						//no collision
					}
					else
					{
						creature[j].health -= 10;
						
						if (creature[j].health <= 0)
						{
							creature[j].alive = false;
							player.explode(creature[j].x + (grid_size / 2), creature[j].y + (grid_size / 2));
							if (setup.getMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Empty))
							{
								setup.setMapFile((creature[j].x + grid_size / 2) / grid_size, (creature[j].y + grid_size / 2) / grid_size, setup.Gold);
							}
						}
					}
				}
			}
		}
	}

}

int Enemy::getMaxBullets()
{
	return max_bullets;
}




#include<iostream>
#include<sstream>
#include<allegro.h>
#include<ctime>
#include <string>
#include "ScreenManager.h"
#include "Player.h"
#include "Setup.h"
#include "Enemy.h"


using namespace std;

volatile long counter = 0;
void Increment()
{
    counter ++;
}

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "A");
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_width, screen_height, 0, 0);

    LOCK_VARIABLE(counter);
    LOCK_FUNCTION(Increment);
    install_int_ex(Increment, BPS_TO_TIMER(100));

    bool done = false;

	Setup setup;
	Player player;
	Enemy enemy;

	class Player;
	class Enemy;


	setup.Init(player, enemy);

	BITMAP *Buffer = create_bitmap(setup.getMapSizeX()*grid_size, setup.getMapSizeY()*grid_size);

	srand(time(0));

	ScreenManager screenManager;

	screenManager.Init();

    while (!screenManager.getDone())
    {
        while(counter > 0)
        {

            // Update
			screenManager.Update(Buffer);
            counter --;
        }
        // Draw
		screenManager.Draw(Buffer);
        clear_bitmap(Buffer);
    }
	destroy_bitmap(Buffer);
    return 0;

}
END_OF_MAIN()
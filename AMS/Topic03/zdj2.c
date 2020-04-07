#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// ADD INCLUDES HERE

// Configuration
#define DELAY (10) /* Millisecond delay between game updates */

#define HERO_WIDTH (5)
#define HERO_HEIGHT (5)
#define ZOMBIE_WIDTH (5)
#define ZOMBIE_HEIGHT (5)

// Game state.
bool game_over = false;	/* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */

char *hero_image =
	/**/ "H   H"
		 /**/ "H   H"
		 /**/ "HHHHH"
		 /**/ "H   H"
		 /**/ "H   H";

char *zombie_image =
	/**/ "ZZZZZ"
		 /**/ "   Z "
		 /**/ "  Z  "
		 /**/ " Z   "
		 /**/ "ZZZZZ";

char *msg_image =
	/**/ "Goodbye and thank-you for playing ZombieDash Jr."
		 /**/ "            Press any key to exit...            ";

// (c) Declare a sprite_id called hero.
sprite_id hero;

// (m) Declare a sprite_id called zombie.
sprite_id zombie;

void draw_border()
{
	draw_line(0, 0, screen_width() - 1, 0, '*');
	draw_line(0, 0, 0, screen_height() - 1, '*');
	draw_line(0, screen_height() - 1, screen_width() - 1, screen_height() - 1, '*');
	draw_line(screen_width() - 1, 0, screen_width() - 1, screen_height() - 1, '*');
}

sprite_id setup_zombie()
{
	int zx = rand() % (screen_width() - 5 - 2) + 1;
	int zy = rand() % (screen_height() - 5 - 2) + 1;
	sprite_id new_zombie = sprite_create(zx, zy, 5, 5, zombie_image);
	sprite_turn_to(new_zombie, 0.1, 0);
	int angle = rand() % 360;
	sprite_turn(new_zombie, angle);

	return new_zombie;
}

void draw_all()
{
	clear_screen();
	sprite_draw(hero);
	sprite_draw(zombie);
	draw_border();
	show_screen();
}
// Setup game.
void setup(void)
{
	hero = sprite_create((screen_width() - 5) / 2, (screen_height() - 5) / 2, 5, 5, hero_image);
	srand(5198985);
	zombie = setup_zombie();
	draw_all();
}

bool paused = false; // Flag value, either true or false / toggle

int read_char()
{
	int key_code = paused ? wait_char() : get_char();

	if ('p' == key_code)
	{
		paused = !paused; // Toggles paused
	}

	return key_code;
}

void move_hero(int key)
{
	int hx = round(sprite_x(hero));
	int hy = round(sprite_y(hero));

	if (key == 'a' && hx > 1)
	{
		sprite_move(hero, -1, 0);
	}
	else if (key == 'd' && hx < (screen_width() - 1) - sprite_width(hero))
	{
		sprite_move(hero, +1, 0);
	}
	else if (key == 's' && hy < (screen_height() - 1) - sprite_height(hero))
	{
		sprite_move(hero, 0, +1);
	}
	else if (key == 'w' && hy > 1)
	{
		sprite_move(hero, 0, -1);
	}
}

void auto_move(sprite_id sid, int keyCode)
{
	if (keyCode == 'z' || keyCode < 0)
	{
		sprite_step(zombie);

		int zx = round(sprite_x(sid));
		int zy = round(sprite_y(sid));

		// (t) Get the displacement vector of the sid.
		double zdx = sprite_dx(sid);
		double zdy = sprite_dy(sid);
		// (u) Test to see if the zombie hit the left or right border.
		if (zx == 0 || zx == screen_width() - 1)
		{
			zdx = -zdx;
		}
		// (v) Test to see if the zombie hit the top or bottom border.
		if (zy == 0 || zy == screen_height() - 1)
		{
			zdy = -zdy;
		}
		// (w) Test to see if the zombie needs to step back and change direction.
		if (zdx != sprite_dx(sid) || zdy != sprite_dy(sid))
		{
			sprite_back(sid);
			sprite_turn_to(sid, zdx, zdy);
		}
	}
}

bool sprites_collide(sprite_id s1, sprite_id s2)
{
	int l1 = round(sprite_x(s1));
	int l2 = round(sprite_x(s2));
	int t1 = round(sprite_y(s1));
	int t2 = round(sprite_y(s2));
	int r1 = l1 + sprite_width(s1) - 1;
	int r2 = l2 + sprite_width(s2) - 1;
	int b1 = t1 + sprite_height(s1) - 1;
	int b2 = t2 + sprite_height(s2) - 1;

	if ( l1 > r2 ) return false;
	if ( l2 > r1 ) return false;
	if ( t1 > b2 ) return false;
	if ( t2 > b1 ) return false;

	return true;
}

void end_game(){
	game_over = true;
	static char * msg_text = "Game over!You WIN!";
	int msg_width = strlen(msg_text) / 2;
	int msg_height = 2;
	int msg_x = (screen_width() - msg_width) / 2;
	int msg_y = (screen_height() - msg_height) /2;
	sprite_id msg = sprite_create(msg_x,msg_y,msg_width,msg_height,msg_text);
	clear_screen();
	sprite_draw(msg);
	show_screen();
	timer_pause(1000);

	while(get_char() >= 0) // Purge input buffer
	{
		// Do Nothing
	}

	wait_char();
}

// Play one turn of game.
void process(void)
{
	int keyCode = read_char();

	if ( keyCode == 'q'){
		end_game();
	}

	move_hero(keyCode);
	auto_move(zombie, keyCode);

	if (sprites_collide(hero,zombie)){
		end_game();
	}
	draw_all();
}

// Clean up game
void cleanup(void)
{
	// STATEMENTS
}

// Program entry point.
int main(void)
{
	setup_screen();
	setup();
	show_screen();

	while (!game_over)
	{
		process();

		if (update_screen)
		{
			show_screen();
		}

		timer_pause(DELAY);
	}

	cleanup();

	return 0;
}
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/********************************
   Race To Zombie Mountain Game
   By Kevin Duong
   Student ID: n9934731
*********************************/

/* Game state. */
// BOOLEAN STATEMENTS
bool game_over = false;
bool update_screen = true;
bool menu = true;
bool retry = true;
bool pause = false;

bool pauseTest = false;

// INT HUD
int health = 100;
int fuel = 100;
int distance = 0;
int speed = 0;

// TIME
time_t seconds = 0;
time_t minutes = 0;
time_t delay_count = 0;
time_t fuel_count = 0;
time_t distance_count = 0;

time_t refuel_timer = 0;
time_t refuel_count = 0;

/*-------------*/
								
/* Declare sprite IDs here. */
/* No. of sprites needed for the game. */
sprite_id player;

#define MAX_TREES (100)
sprite_id tree[MAX_TREES];
#define MAX_CROSS (100)
sprite_id cross[MAX_CROSS];
#define MAX_BUILDINGS  (10)
sprite_id house[MAX_BUILDINGS];
sprite_id church[MAX_BUILDINGS];

#define MAX_FUEL (10)
sprite_id fuel_depot_left[MAX_FUEL];
sprite_id fuel_depot_right[MAX_FUEL];

#define MAX_ZOMBIES (100)
sprite_id zombieWave[MAX_ZOMBIES];
#define MAX_ZM (4)
sprite_id zombieWaveZM[MAX_ZM];

sprite_id zombie_mountain;

/*--------------------------*/

/* SPRITE CREATION */

sprite_id create_zombie_mountain( void )
{
	char * zm_image =  




  "                                Z                               "
  "                      zZz      Z Z        Z                     "
  "                     Z   Z    Z^.zz__   _Z Z                    "
  "        z        .zz'Zzz_ Z__Z.      Z Z    Z  zz  ZZZ          "
  "       Z Zz    zZ ^      ZZ  ZZ  :'   ZZZZ  ZZ  ZZZ   Z         "
  "      Z    Z  Z    .'   _Z  Z  Z   ^ Z    zz  zz .`'z_zZ        "
  "     ZZzz  zzZ :' ZZ  ^Z  ^Z    `zz.Z.'  ^  `-.Z _    _:Z z     "
  "    Z    zz  Z  _Z  Z-' __Z.' ^ _   Z_   .'Z   _Z Z .  zzZ Z    "
  "  ZZ  zzz  zz zz     Z Z -.   _Z Z -. `_Z   Z Z  z `.zZ  ^  Z   "
  " Z  zzz__ z   z .-'.--'    . Z    `zz.Z .z'  `-.  `Zz `z  zz zz "
  "Z        zz  z z      `-.   z  .-'   z .   .'   z    z  z  zz  z";
				
	int zmx = (screen_width()-64)/2;
	int zmy = -20 ;
	
	sprite_id zombie_mountain = sprite_create(zmx, zmy, 64, 11, zm_image);
	sprite_draw( zombie_mountain );
	
	return zombie_mountain;
}

sprite_id create_zombieZM( void )
{
	char * zombie_image =   "ZZZZZZ"
							"    Z "
							"   Z  "
							"  Z   "
							" Z    "
							"ZZZZZZ";
	
	int zx = (rand() % ( ( ((screen_width() + 14) /2) + 1 - ((screen_width() + 3)/3) ) ) ) + (screen_width() + 3)/3;
	int zy = -6;
	
	sprite_id zombie = sprite_create(zx, zy, 6, 6, zombie_image);
	sprite_draw( zombie );
	
	sprite_turn_to( zombie, 1, 1 );
	
	return zombie;
}

sprite_id create_miniZombie( void )
{
	char * zombie_image =   "ZZZZ"
						    "  Z "
							" Z  "
							"ZZZZ";
	
	int zx = (rand() % ( ( ((screen_width() + 14) /2) + 1 - ((screen_width() + 3)/3) ) ) ) + (screen_width() + 3)/3;
	int zy = (rand() % ( ( -100 - -3000 ) ) ) + -3000;
	
	sprite_id zombie = sprite_create(zx, zy, 4, 4, zombie_image);
	sprite_draw( zombie );
	
	double zdx = rand();
	double zdy = rand();
	double magnitude = sqrt( zdx * zdx + zdy * zdy);
	zdx /= magnitude; // zdx = zdx / magnitude;
	zdy /= magnitude;
	
	sprite_turn_to( zombie, zdx, zdy );
	
	return zombie;
}

sprite_id create_station_left( void )
{
	char * fuel_image_left 	= 	" ___ "
								"|[F]|"
								"| o=>"
								"|   |"
								"~~~~~";
				
	int fx = (screen_width()-16)/3;
	int fy = (rand() % ( ( screen_height() + 1 - -2000 ) ) ) + -2000; 
	
	sprite_id fuel = sprite_create(fx, fy, 5, 5, fuel_image_left);
	sprite_draw( fuel );
	
	return fuel;
}

sprite_id create_station_right( void )
{
	char * fuel_image_right = 	" ___ "
								"|[F]|"
								"<=o |"
								"|   |"
								"~~~~~";
						
	int fx = (screen_width() + 1 )/1.5;
	int fy = (rand() % ( ( screen_height() + 1 - -2000 ) ) ) + -2000; 
	
	sprite_id fuel = sprite_create(fx, fy, 5, 5, fuel_image_right);
	sprite_draw( fuel );
	
	return fuel;
}

sprite_id create_tree( void )
{
	char * tree_image =   " ___ "
						  "|###|"
						  "|###|"
						  " | | "
						  " |@| "
						  "~~~~~";
						 
	//int tx = (screen_width()-33)/3; //max range
	//int tx = 1; //min range
	int tx = (rand() % ( ( ((screen_width()-33)/3) + 1 - 1 ) ) ) + 1;
	int ty = (rand() % ( ( screen_height() + 1 - -3000 ) ) ) + -3000;
	
	sprite_id tree = sprite_create(tx, ty, 5, 6, tree_image);
	sprite_draw( tree );
	
	return tree;
}

sprite_id create_house( void )
{
	char * house_image =
	
" %%%%%%%%%%%%%%%%% "
"%%%%%%%%%%%%%%%%%%%"
" ||  ___   ___  || "
" ||  |_|   |_|  || "
" ||  ***   ***  || "
" ||  ____  ___  || "
" ||  |@@|  |_|  || "
" ||  |@*|  ***  || "
"~~~~~~~~~~~~~~~~~~~";
						 
	int hx = (rand() % ( ( ((screen_width()-76)/3) + 1 - 1 ) ) ) + 1;
	int hy = (rand() % ( ( screen_height() + 1 - -3000 ) ) ) + -3000;
	
	sprite_id house = sprite_create(hx, hy, 19, 9, house_image);
	sprite_draw( house );
	
	return house;
}

sprite_id create_church( void )
{
	char * church_image =
	
 "             +   "
 "            (|)  "
 "       ^   .|_|. "
 "      ^+^  |===| "
 "     ^+++^ | o | "
 "   _^__+__^| _ | "
 "   |=======||-|| "
 "  /| . . . |___| " 
 " | | [] [] |   | "
 " | | . . . |   | " 
 "~~~~~~~~~~~~~~~~~";
						 
	int chx = (rand() % ( ( (screen_width() - 18) + 1 - ((screen_width() + 10)/3*2) ) ) ) + (screen_width() + 10)/3*2;
	int chy = (rand() % ( ( screen_height() + 1 - -3000 ) ) ) + -3000;
	
	sprite_id church = sprite_create(chx, chy, 17, 11, church_image);
	sprite_draw( church );
	
	return church;
}

sprite_id create_cross( void )
{
	char * cross_image  =   "  |  "
							"--o--"
							"  |  "
							"  |  "
							" ~~~ ";
	
	int cx = (rand() % ( ( (screen_width() - 6) + 1 - ((screen_width() + 10)/3*2) ) ) ) + (screen_width() + 10)/3*2;
	int cy = (rand() % ( ( screen_height() + 1 - -3000 ) ) ) + -3000; 
	
	sprite_id cross = sprite_create(cx, cy, 5, 5, cross_image);
	sprite_draw( cross );
	
	return cross;
}
/*-----------------*/

/* Setup characters and scenery. */
void setup_game( void ) 
{
	
	char * player_image =   "[]=[]"
							"  |  "
							" |O| "
							" |-| "
							"[]=[]";
	
	setup_screen();
	
	int w = screen_width();
	int h = screen_height();
	
	player = sprite_create( (w-5)/2, (h-5)/1.25, 5, 5, player_image );
	sprite_draw(player);
	
	/* No. of sprites needed for game. */
	for ( int i = 0; i < 5; i++ ) 
	{
		fuel_depot_left[i] = create_station_left();
		fuel_depot_right[i] = create_station_right();
	}
	
	for ( int i = 0; i < 10; i++ ) 
	{
		house[i] = create_house();
		church[i] = create_church();
	}
	
	for ( int i = 0; i < 100; i++ ) 
	{
		tree[i] = create_tree();
		cross[i] = create_cross();
	}
	
	for ( int i = 0; i < 100; i++ ) 
	{
		zombieWave[i] = create_miniZombie();
	}
	
	for ( int i = 0; i < 4; i++ ) 
	{
		zombieWaveZM[i] = create_zombieZM();
	}
	
	zombie_mountain = create_zombie_mountain();
	
	show_screen();
}
/*-------------------------------*/

/* Zombie Map. */
void setup_map()
{
	int w = screen_width();
	int h = screen_height();
	
	/* Top border. */
	draw_line(0, 0, w-1, 0, '@');
	/* Top_bottom border. */
	draw_line(0, 2, w-2, 2, '@');
	/* Bottom border. */
	draw_line(0, h-1, w-1, h-1, '@');
	/* Left border. */
	draw_line(0, 0, 0, h-1, '@');
	/* Right border. */
	draw_line(w-1, 0, w-1, h-1, '@');
	
	/* Road left side. */
	draw_line((w-1)/3, 3, (w-1)/3, h-2, '|');
	/* Road right side. */
	draw_line((w-1)/1.5, 3, (w-1)/1.5, h-2, '|');
}
/*-------------*/

/* Main menu for the game. */
void main_menu() 
{
	int w = (screen_width() - 44) / 2;
	int h = (screen_height() - 14) / 2;
	
    char * display[] = 
	{ 
	  " __________________________________________ ", 
	  "|    Welcome to Race To Zombie Mountain    |", 
	  "|                                          |",
	  "|              By Kevin Duong              |", 
	  "|           Student ID: n9934731           |", 
	  "|                                          |",
      "|             Keyboard Hotkeys             |",
	  "|              W,S -  up/down              |", 
	  "|             A,D - left/right             |", 
	  "|                P -  pause                |", 
	  "|                 Q - quit                 |",
	  "|                                          |",
	  "|     Distance to Zombie Mountain: 6km     |",
	  "|                                          |",
	  "|          Press any key to start          |",
	  "|__________________________________________|",
	};
	
    for (int i = 0; i <= 15; i++) 
	{
      draw_string(w, h+i, display[i]);
    }

	show_screen();
}
/*-------------------------*/

/* Player HUD. */
void player_hud() 
{
	int w = screen_width() / 4;
	int h = screen_height() / 4;
	
	/* Condition of the player. */
	draw_formatted(2, 1, " Condition: %d", health);
	/* Fuel of the race car. */
	draw_formatted(w * 1, 1, " Fuel: %d%%", fuel);
	/* Total distance traveled. */
	draw_formatted(w * 2, 1, " Distance: %dm", distance);
	/* Time of the game. */
	draw_formatted(w * 3, 1, "Time: %02d:%02d", minutes, seconds);
	/* Current speed of the race car. (1 - 10) */
	draw_formatted(2, h * 3.75, " Speed: %d", speed);
	
	draw_formatted(w * 3, h * 3.75, " Refuel: %d", refuel_timer);
	
	show_screen();
}
/*-------------*/

/* Countdown start game. */
void countdown() 
{
	int w = screen_width();
	int h = screen_height();

	draw_string((w-12)/2, (h-2)/2, "Get ready...");
	for (int i = 3; i > 0; i--) 
	{
		draw_formatted((w-1)/2, (h+2)/2, "%d", i);
		show_screen();
		timer_pause(1000);
	}
}
/*-----------------------*/

/* Real-time clock. */
void clock() 
{
	delay_count++;
	if (delay_count == 35) 
	{
		seconds++;
		delay_count = 0;
		
		if (seconds == 60) 
		{
			seconds = 0;
			minutes++;
		}
	}
}
/*------------------*/

/* Global integer settings. */
void game_reset(int key)
{
	if ( key == 'y') 
	{
		game_over = false;
		retry = true;
		minutes = 0;
		seconds = 0;
		health = 100;
		fuel = 100;
		distance = 0;
		speed = 0;
		menu = true;
		fuel_count = 0;
		distance_count = 0;
		delay_count = 0;
	} 
	else 
	{
		game_over = true;

	}
}

/* Game over function. */
void display_gameover()
{
	char * gameover_image 	=	"    You are dead    "
								"  Play again?  y/n  ";
								
	/* Wipes game play screen. */
	clear_screen();
	/* Draw game over message. */
	sprite_draw(sprite_create( (screen_width()-20)/2, screen_height()/2, 20, 2, gameover_image ));
	/* Display message. */
	show_screen();
	
	int key = wait_char();
	game_reset(key);
	
}
/*---------------------*/

/* Win the game function. */
void display_finish()
{
	/* Wipes game play screen. */
	clear_screen();
	
	draw_string((screen_width() - 58)/2, screen_height()/2 - 2 ,"Congratulations! You have finally reached Zombie Mountain.");
	
	draw_formatted((screen_width() - 30)/2, screen_height()/2 ,"    Distance Traveled: %dm    ", distance);
	
	draw_formatted((screen_width() - 30)/2, screen_height()/2 + 1, "    Time Elapsed: %02d:%02d   ", minutes, seconds);
	
	draw_string((screen_width() - 16)/2, screen_height()/2 + 3 ,"Play again?  y/n");
								
								
	/* Display message. */
	show_screen();
	
	int key = wait_char();
	game_reset(key);
}
/*------------------------*/


/* Fuel conditions. */
void fuel_consumption()
{
	/* When fuel runs out. */
	if (fuel == 0)
	{
		display_gameover();
	}
	
	else if (speed == 0)
	{
		return;
	}
	else
	{
		fuel_count++;
	}
	
	/* Consumption of fuel based on speed. */
	if ((speed == 1 || speed == 2) && (fuel_count >= 50))
	{
		fuel --;
		fuel_count = 0;
	}
	
	if ((speed == 3 || speed == 4) && (fuel_count >= 40))
	{
		fuel --;
		fuel_count = 0;
	}
	
	if ((speed == 5 || speed == 6) && (fuel_count >= 30))
	{	
		fuel --;
		fuel_count = 0;
	}
	
	if ((speed == 7 || speed == 8) && (fuel_count >= 20))
	{
		fuel --;
		fuel_count = 0;
	}
	
	if ((speed == 9) && (fuel_count >= 10))
	{
		fuel --;
		fuel_count = 0;
	}
	
	if ((speed == 10) && (fuel_count >= 5))
	{
		fuel --;
		fuel_count = 0;
	}
}
/*------------------*/

/* Distance traveled. */
void distance_traveled()
{	
	/* Condition to count distance. */
	if (speed == 0)
	{
		return;
	}
	else 
	{
		distance_count++;
	}
	
	/* Speed variations. */
		
	/* Speed: 14km/h */
	if ((speed == 1) && (distance_count >= 8))
	{	
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 18km/h */
	if ((speed == 2) && (distance_count >= 7))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 25km/h */
	if ((speed == 3) && (distance_count >= 5))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 29km/h */
	if ((speed == 4) && (distance_count >= 4))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 40km/h */
	if ((speed == 5) && (distance_count >= 3))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 61km/h */
	if ((speed == 6) && (distance_count >= 2))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 126km/h */
	if ((speed == 7) && (distance_count >= 1))
	{
		distance ++;
		distance_count = 0;
	}
	
	/* Speed: 252km/h */
	if ((speed == 8) && (distance_count >= 0))
	{
		distance += 2;
		distance_count = 0;
	}
	
	/* Speed: 378km/h */
	if ((speed == 9) && (distance_count >= 0))
	{
		distance += 3;
		distance_count = 0;
	}
	
	/* Speed: 504km/h */
	if ((speed == 10) && (distance_count >= 0))
	{
		distance += 4;
		distance_count = 0;
	}
}

/*--------------------*/

/* Motion scenery relative to speed of car. */
void scenery_stationary(sprite_id sprite)
{
	sprite_step( sprite );
	if (speed == 1)
	{
		sprite_turn_to( sprite, 0, 0.1 );
	}
	
	else if (speed == 2)
	{
		sprite_turn_to( sprite, 0, 0.2 );
	}
	
	else if (speed == 3)
	{
		sprite_turn_to( sprite, 0, 0.3 );
	}
	
	else if (speed == 4)
	{
		sprite_turn_to( sprite, 0, 0.4 );
	}
	
	else if (speed == 5)
	{
		sprite_turn_to( sprite, 0, 0.5 );
	}
	
	else if (speed == 6)
	{
		sprite_turn_to( sprite, 0, 0.6 );
	}
	
	else if (speed == 7)
	{
		sprite_turn_to( sprite, 0, 0.7 );
	}
	
	else if (speed == 8)
	{
		sprite_turn_to( sprite, 0, 0.8 );
	}
	
	else if (speed == 9)
	{
		sprite_turn_to( sprite, 0, 0.9 );
	}
	
	else if (speed == 10)
	{
		sprite_turn_to( sprite, 0, 1 );
	}
	
	else
	{
		sprite_turn_to( sprite, 0, 0 );
	}
}

/* Game paused. */
void pause_mode ()
{
	int w = screen_width();
	int h = screen_height();
	
	sprite_draw(player);
	
	draw_string((w - 11)/2, h/2, "Game Paused");
	show_screen();
	
	// Wait for key to continue.
	wait_char();
	// Turn off menu.
	pause = false;
	for (int i = 3; i > 0; i--) 
	{
		draw_formatted((w-1)/2, (h+4)/2, "%d", i);
		show_screen();
		timer_pause(1000);
	}
}

void testCapture () 
{
	sprite_draw(player);
	show_screen();
	/* Press v to screenshot. */
	wait_char();
	pauseTest = false;
}
/*--------------*/

void zombie_movement( sprite_id zombie ) 
{	
	sprite_step( zombie );
	
	int zx = round(sprite_x(zombie));
	
	if ( (zx < screen_width()/3) || (zx > (screen_width()-9)/1.5) ) 
	{
		double zdx = sprite_dx ( zombie); 
		double zdy = sprite_dy( zombie);
		sprite_turn_to( zombie, -zdx, zdy);
		
	}
}

/* COLLISION SETTINGS */
bool collided ( sprite_id player, sprite_id sprite ) 
{
	int top_player = round(sprite_y(player));
	int left_player = round(sprite_x(player));
	int right_player = left_player + sprite_width(player) - 1;
	int bottom_player = top_player + sprite_height(player) - 1;
	
	int top_object = round(sprite_y(sprite));
	int left_object = round(sprite_x(sprite));
	int right_object = left_object + sprite_width(sprite) - 1;
	int bottom_object = top_object + sprite_height(sprite) - 1;
	
	if ( top_object > bottom_player ) return false;
	if ( top_player > bottom_object ) return false;
	if ( left_player > right_object ) return false;
	if ( left_object > right_player ) return false;
	
	return true;
}

bool refuel_left ( sprite_id player, sprite_id fuel_depot_left ) 
{
	int top_player = round(sprite_y(player));
	int left_player = round(sprite_x(player));
	int right_player = left_player + sprite_width(player) - 1;
	int bottom_player = top_player + sprite_height(player) - 1;
	
	int top_fuel_l = round(sprite_y(fuel_depot_left));
	int left_fuel_l = round(sprite_x(fuel_depot_left));
	int right_fuel_l = left_fuel_l + sprite_width(fuel_depot_left);
	int bottom_fuel_l = top_fuel_l + sprite_height(fuel_depot_left) - 5;
	
	if ( top_fuel_l > bottom_player ) return false;
	if ( top_player > bottom_fuel_l ) return false;
	if ( left_player > right_fuel_l ) return false;
	if ( left_fuel_l > right_player ) return false;
	
	return true;
}

bool refuel_right ( sprite_id player, sprite_id fuel_depot_right ) 
{
	int top_player = round(sprite_y(player));
	int left_player = round(sprite_x(player));
	int right_player = left_player + sprite_width(player) - 1;
	int bottom_player = top_player + sprite_height(player) - 1;
	
	int top_fuel_r = round(sprite_y(fuel_depot_right));
	int left_fuel_r = round(sprite_x(fuel_depot_right) - 1);
	int right_fuel_r = left_fuel_r + sprite_width(fuel_depot_right) - 1;
	int bottom_fuel_r = top_fuel_r + sprite_height(fuel_depot_right) - 5;
	
	if ( top_fuel_r > bottom_player ) return false;
	if ( top_player > bottom_fuel_r ) return false;
	if ( left_player > right_fuel_r ) return false;
	if ( left_fuel_r > right_player ) return false;
	
	return true;
}
/*------------------------*/	

/* Player control. */
void player_controls ( int key ) 
{
	switch (key) 
	{
		/* Pause the game. */
		case 'p':
		pause = true;
		return;
		
		//TEST CASE FEATURE
		case 'v':
		pauseTest = true;
		return;
		/*----------------*/
		
		/* Quit the game. */
		case 'q':
		display_gameover();
		return;
		/*----------------*/
	
		/* Player moves left. */
		case 'a': 
		if (speed == 0)
		{
			return;
		}
		else
		{
			if( sprite_x(player) > 1 ) 
			{
				sprite_move( player, -1, 0);
			}
		}
		break;
		/*---------------------*/
	
		/* Player moves right. */
		case 'd':
		if (speed == 0)
		{
			return;
		}
		else
		{
			if( sprite_x(player) < screen_width() - 6 ) 
			{
				sprite_move( player, 1, 0);
			}
		}
		break;
		/*---------------------*/
	
		/* Player moves up. */
		case 'w':
		if (speed == 10 || fuel == 0)
		{
			return;
		}
		else 
		{
			speed++;
		}
		break;
		/*------------------*/
	
		/* Player moves down. */
		case 's':
		if (speed == 0)
		{
			return;
		}
		else
		{
			speed--;
		}
		break;
		/*--------------------*/
	}	
}
/*----------------*/

void damage_function ()
{
	char * player_image =   "[]=[]"
							"  |  "
							" |O| "
							" |-| "
							"[]=[]";		
	
	health -= 10;
		
	fuel = 100;
	fuel_count = 0;
	speed = 0;
	
	refuel_timer = 0;
	refuel_count = 0;
		
	player = sprite_create( rand() % (screen_width()-5), (screen_height()-5)/1.25, 5, 5, player_image );
	sprite_draw(player);
}

void refuel_function ()
{
	speed = 0;
	refuel_count++;
	if (refuel_count >= 35) 
	{
		refuel_timer++;
		refuel_count = 0;
	}

	if (refuel_timer == 3)
	{
		fuel = 100;
		for ( int i = 0; i < 5; i++ ) 
		{
			if ( refuel_left(player, fuel_depot_left[i]))
			{	
				sprite_move( player, 1, 0);
			}
			
			if ( refuel_right(player, fuel_depot_right[i]) )
			{	
				sprite_move( player, -1, 0);
			}
		}
		
		refuel_timer = 0;
		refuel_count = 0;
	}
}

/* Player conditions */
void player_conditions ()
{
	/* Collision with zombie, scenery. */
	for ( int i = 0; i < 100; i++ ) 
	{
		if ( collided(player, cross[i]) || collided(player, tree[i]) || collided( player, zombieWave[i]) )
		{	
			damage_function();
		}
	}
	
	for ( int i = 0; i < 10; i++ ) 
	{
		if ( collided(player, house[i]) || collided(player, church[i]) )
		{	
			damage_function();
		}
	}
	
	for ( int i = 0; i < 5; i++ ) 
	{
		if ( health <= 0 || collided(player, fuel_depot_left[i]) || collided(player, fuel_depot_right[i]) )
		{	
			display_gameover();
		}
		
		if ( refuel_left(player, fuel_depot_left[i]) || refuel_right(player, fuel_depot_right[i]) )
		{	
			refuel_function();
		}
		
	}
		
	for ( int i = 0; i < 3; i++ ) 
	{
		if ( collided( player, zombieWaveZM[i] ) ) 
		{
			display_gameover();
		}
	}
	
	if (collided( player, zombie_mountain) || ( distance > 6500 ) )
	{
		display_finish();
	}
	/*---------------------------------*/
	
	/* Boundary condition. */
	if ( ((sprite_x(player) < (screen_width()-1)/3) || (sprite_x(player) > (screen_width()-6)/1.5)) && (speed > 3) ) 
	{
		speed = 3;
	}
	/*--------------------*/
}
void object_movement()
{
	for ( int i = 0; i < 5; i++ ) 
	{
		scenery_stationary(fuel_depot_left[i]);
		scenery_stationary(fuel_depot_right[i]);
	}
	
	for ( int i = 0; i < 100; i++ ) 
	{
		scenery_stationary(tree[i]);
		scenery_stationary(cross[i]);
	}
	
	for ( int i = 0; i < 10; i++ ) 
	{
		scenery_stationary(house[i]);
		scenery_stationary(church[i]);
	}
	
	/* ZOMBIE SPAWN POINT */		
	for ( int i = 0; i < 100; i++ ) 
	{
		zombie_movement(zombieWave[i]);
	}

	if ( distance > 6000 )
	{
		for ( int i = 0; i < 4; i++ ) 
		{
			zombie_movement(zombieWaveZM[i]);
		}
	}
	/* END ZOMBIE SPAWN POINT */
}

void object_spawn()
{
	for ( int i = 0; i < 5; i++ ) 
	{
		sprite_draw(fuel_depot_left[i]);
		sprite_draw(fuel_depot_right[i]);
	}
	
	for ( int i = 0; i < 100; i++ ) 
	{
		sprite_draw(tree[i]);	
		sprite_draw(cross[i]);
	}
	
	for ( int i = 0; i < 10; i++ ) 
	{
		sprite_draw(house[i]);
		sprite_draw(church[i]);
	}
	
	/* ZOMBIE SPAWN POINT */		
	for ( int i = 0; i < 100; i++ ) 
	{
		sprite_draw(zombieWave[i]);
	}

	if ( distance > 6000 )
	{
		for ( int i = 0; i < 4; i++ ) 
		{
			sprite_draw(zombieWaveZM[i]);
		}
	}
	/* END ZOMBIE SPAWN POINT */
}

/* Process the game. */
void process( void )
{
	//ADD CONDITIONS HERE
	int key = get_char();

	player_controls (key);
	player_conditions (key);
	object_movement();
	clear_screen();
	
	object_spawn();
	
	setup_map();
	
	if (distance >= 6000)
	{
		scenery_stationary( zombie_mountain );
		sprite_draw(zombie_mountain);
	}
	
	sprite_draw(player);
	
	player_hud();
	fuel_consumption();
	distance_traveled();
	clock();
}
/*-------------------*/

/* Program entry point. */
int main(void) 
{
	setup_game();
	
	while ( !game_over ) 
	{
		// Display scene.
		setup_map();
		
		// Player's hud.
		player_hud();
		
		if (menu) 
		{
			// Display main menu.
			main_menu();
			// Wait for key to continue.
			wait_char();
			// Turn off menu.
			menu = false;
			// Clear screen.
			clear_screen();
			// Initiate countdown.
			countdown();
		}
		
		if (retry) 
		{
			// Respawn.
			setup_game();
			// Turn off retry.
			retry = false;
		}
		
		if (pause)
		{	
			pause_mode();
		}
		
		if (pauseTest)
		{	
			testCapture();
		}
		
		// Process game.
		process();
		
		if (update_screen) 
		{
			show_screen();
		}

		timer_pause(30);
	}
}
/*----------------------*/
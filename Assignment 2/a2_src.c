#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <lcd.h>

#include <stdbool.h>
#include <graphics.h>
#include <sprite.h>
#include <macros.h>
#include "lcd_model.h"

//#include "usb_serial.h"


/* DEFINITIONS -----------------------------------------------------------------------*/

#define PLAYER_H (8)
#define PLAYER_W (8)

#define PLATFORM_H (2)
#define PLATFORM_W (10)

#define TREASURE_H (3)
#define TREASURE_W (8)

#define MAX_PLATFORMS (40)
#define COLUMN_W (PLATFORM_W + 2)
#define ROW_H (PLATFORM_H + PLAYER_H)
#define CELL_SPAWN_PERCENT (70)
#define FORBIDDEN_SPAWN_PERCENT (25)

#define MAX_ZOMBIES (5)
#define MAX_FOOD (10)

/* STRUCT SETUP ----------------------------------------------------------------------*/

// Game struct containing game states
typedef struct Game {
    bool start_screen;
    bool paused;
    bool over;
    bool end_screen;
    int seed;
} Game;

typedef struct Timer {
	unsigned int delay_count;
	unsigned int minutes;
	unsigned int seconds;
} Timer;

// Player contains sprite, lives, score and food.
typedef struct Player {
    Sprite sprite;
    unsigned int lives;
    unsigned int score;
    unsigned int food;
} Player;

// Platform includes sprite, num_platforms on screen.
typedef struct Platforms {
    Sprite sprites[MAX_PLATFORMS];
    unsigned int num;
	unsigned int num_forbidden;
    bool safe[MAX_PLATFORMS];
} Platforms;

// Treasure.
typedef struct Treasure {
    Sprite sprite;
	bool move;
	bool is_right;
} Treasure;

// Food with sprite and num_food;
typedef struct Food {
    Sprite sprite;
    unsigned int num;
} Food;

// Zombies with sprite and num_zombies.
typedef struct Zombies {
    Sprite sprite[MAX_ZOMBIES];
    unsigned int num_zombies;
} Zombies;

// Intialise Structs
Timer refreshRate;
/* Sprites ---------------------------------------------------------------------------*/

static uint8_t player_img [] = {
    0b00111100,
	0b00100100,
	0b00111100,
	0b00011000,
	0b01111110,
	0b01011010,
	0b00011000,
	0b00111100,
};

static uint8_t safe_img [] = {
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

static uint8_t forbidden_img [] = {
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
};

static uint8_t treasure_img [] = {
	0b01111110,
	0b11000011,
	0b11111111,
};

static uint8_t food_img [] = {
	0b01010100,
	0b10001010,
	//0b10010,
	//0b01010,
};

/* Timer ------------------------------------------------------------------------------*/
Timer timer;
// Tracks tme played in current game.
void game_timer() {
	timer.delay_count++;
	if (timer.delay_count == 18) 
	{
		timer.seconds++;
		timer.delay_count = 0;
	}

	if (timer.seconds >= 60) {
		timer.seconds = 0;
		timer.minutes += 1;
	}
}


/* HELPER FUNCTIONS --------------------------------------------------------------------*/
char buffer[10];

void draw_int(uint8_t x, uint8_t y, int t) {
	snprintf( buffer, 10, "%d",t );
	draw_string( x, y, buffer, FG_COLOUR );
}

void draw_time(uint8_t x, uint8_t y, int m,int  s) {
	snprintf( buffer, 10, "%02d:%02d", m, s );
	draw_string( x, y, buffer, FG_COLOUR );
}

// Gets current column Location of sprite 
int get_current_col (sprite_id sprite) {
    return (int)round(sprite->x) / COLUMN_W;
}

// Gets current row location of Sprite
int get_current_row (sprite_id sprite) {
    return ((int)round(sprite->y)+ PLATFORM_H ) / ROW_H - 1;
}

// Sprite Collision Detection.
bool sprites_collide (Sprite player, Sprite other_sprite) {
	int top1 = player.y;
    int bottom1 = top1 + player.height - 1;
    int left1 = player.x;
    int right1 = left1 + player.width - 1;

	int top2 = other_sprite.y;
    int bottom2 = top2 + other_sprite.height -1;
    int left2 = other_sprite.x;
    int right2 = left2 + other_sprite.width - 1;

	return !( (top1 > bottom2) || (top2 > bottom1) || (right1 < left2) || (right2 < left1));

}


/* Setup ------- ---------------------------------------------------------------------*/

// Initialises teensy pew pew, sets CPU speed and initalizes display.
void initializeTeensy (void) {
	set_clock_speed(CPU_8MHz);
	
    // setup the LCD screen
	lcd_init(LCD_DEFAULT_CONTRAST);
}

// Setup Game
Game game;
void intialSetup () {
    game.start_screen = true;
    game.paused = false;
    game.over = false;
    game.end_screen = false;
    //game.seed = rand() % rand() %(100);
	game.seed = 1;
}


// Setup Platforms
Platforms platforms;
void setupPlatforms () {
	int w = LCD_X;
	int h = LCD_Y;

	// Compute number of rows and columns possible in game world
    int columns = w / COLUMN_W;
    int rows = (h) / ROW_H;

	 // Compute number of cells that are available
    int available_cells = rows * columns * CELL_SPAWN_PERCENT / 100;
    int total_cells = rows * columns;

	platforms.num = 0;

	int safe_platform_per_col[MAX_PLATFORMS] = {0};

	// Populate Cells in Grid 
	for ( int this_row = 0; this_row < rows; this_row++) {
		for (int this_column = 0; this_column < columns; this_column++) {
			if (platforms.num >= MAX_PLATFORMS) break; // Break the loops and stop creating platforms.
			double spawn_probability = (double) rand () / RAND_MAX;
			if (spawn_probability <= (double)available_cells/total_cells) {
				available_cells --;
				sprite_id this_platform = &platforms.sprites[platforms.num];
				int x = this_column * (w - COLUMN_W) / (columns - 1) + rand() % (COLUMN_W - PLATFORM_W);
				int y = (this_row + 1) * ROW_H - PLATFORM_H;
				sprite_init(this_platform,x,y, PLATFORM_W, PLATFORM_H, safe_img);
				platforms.safe[platforms.num] = true;
				safe_platform_per_col[this_column] ++;
				platforms.num ++;
			} total_cells --;
		}
	}

	platforms.num_forbidden = platforms.num * FORBIDDEN_SPAWN_PERCENT / 100;
	if (platforms.num_forbidden < 2) {
		platforms.num_forbidden = 2;
	} for (int i = 0; i < platforms.num_forbidden; i++) {
		#define attemptsMAX (1000)
		for(int attempt = 0; attempt < attemptsMAX; i++) {
			int platform_index = 1 + rand() % (platforms.num - 1);
            sprite_id this_platform = &platforms.sprites[platform_index];
            int column = get_current_col(this_platform);

            if (safe_platform_per_col[column] > 1 ) {
                platforms.safe[platform_index] = false;
                this_platform->bitmap = forbidden_img;
                break;
            }
		}
	}
	
}

// Gets current platform
int get_current_platform (sprite_id s) {
    int s_left = (int)round(s->x);
    int s_right = s_left + s->width - 1;
    int s_y = (int)round(s->y);

    for (int platform = 0; platform < platforms.num; platform++) {
        sprite_id this_platform = &platforms.sprites[platform];
        int p_left = (int)round(this_platform->x);
        int p_right = p_left + this_platform->width - 1;
        int p_y = (int)round(this_platform->y);

        if (s_right >= p_left && s_left <=p_right && s_y == p_y - s->height) {
            return platform;
        }
    }
    return -1;
}



// Setup Player 
Player player;

// Finds all possible 'safe' spwans for the player, randomly selects between them.
void spawnPlayer() {
    sprite_id starting_platforms[MAX_PLATFORMS];
    int possible_spawns = 0;

    for (int i = 0; i  < platforms.num; i++) {
        sprite_id this_platform = &platforms.sprites[i];
        
        if (platforms.safe[i] && get_current_row(this_platform) == 0) {
            starting_platforms[possible_spawns] = this_platform;
            possible_spawns++;
        }
    }

    int start_index = possible_spawns >= 1 ? rand() % possible_spawns : 0;
    sprite_id starting_platform = starting_platforms[start_index];
    player.sprite.x = starting_platform->x + rand() % (starting_platform->width - player.sprite.width);
    player.sprite.y = starting_platform->y - player.sprite.height;
}

void setupPlayer () {
    sprite_init(&player.sprite, (LCD_X - PLAYER_W) / 2, (LCD_Y) - PLAYER_H - 1, PLAYER_W,PLAYER_H, player_img);
	player.sprite.dy = 0.5; // Vertical acceleration down.
	player.food = 5;
}


// Setup Treasure
Treasure treasure;
void setupTreasure() {
	treasure.move = true;
	sprite_init(&treasure.sprite, (LCD_X - TREASURE_W) / 2, (LCD_Y) - TREASURE_H - 1, TREASURE_W, TREASURE_H, treasure_img); //
}

// Setup Food
Food food;

void setupFood() {
	int x = player.sprite.x - PLAYER_W  / 2;
	int y = player.sprite.y - PLATFORM_H;
	if ( player.food >= 1 ) {
		sprite_init(&food.sprite,x,y,4,4,food_img);
		player.food --;
		food.num ++;
	} else { /* do nothing */}	
}


// Setup Zombies
Zombies zombies;

// Setup Game World
void setupGameWorld () {
	player.score = 0;
    player.lives = 10;
	setupTreasure();
    setupPlayer();
	setupPlatforms();
	spawnPlayer();

}

/* Movement ----------------------------------------------------------------------------*/

// Move the player using the left and right joystick.
void movePlayer () {
    if ( BIT_IS_SET(PINB, 1) ) player.sprite.x += -0.5; // MOVE LEFT
	if ( BIT_IS_SET(PIND, 0) ) player.sprite.x += 0.5; // MOVE RIGHT
	if (BIT_IS_SET(DDRB,7) ) setupFood();
}
// Platforms
 /* not implemnted */

// Treasure
void moveTreasure() {
	if (treasure.move) {
		if (treasure.is_right) {
			treasure.sprite.x += 1;
		} if (treasure.sprite.x >= (LCD_X - TREASURE_W)) {
			treasure.is_right = !treasure.is_right;
		} if (treasure.sprite.x <= 1) {
			treasure.is_right = !treasure.is_right;
		} if (!treasure.is_right){
			treasure.sprite.x -= 1;
		}
	}
}

// Zombies


/* Game Controls -----------------------------------------------------------------------*/

void gameControls () {

    if ( BIT_IS_SET(PINB, 0) ) {
		while (BIT_IS_SET(PINB, 0)) {
			// Block until button released.
		}
		// Button has now been pressed and released...
		game.paused = !game.paused; // toggles pause
	}

	//input for toggling treasure movement
    if ((BIT_IS_SET(PINF, 5)))
    {
		while((BIT_IS_SET(PINF, 5))) {
			// Block until button released.
		}
		// Button has now been pressed and released...
        treasure.move = !treasure.move;
    };
}

/* Draw ---------------------------------------------------------------------------------*/
void drawAll() {
	clear_screen();
	sprite_draw(&treasure.sprite);
	sprite_draw(&player.sprite);

	for (int i = 0; i < platforms.num; i++)
    {
        sprite_draw(&platforms.sprites[i]);
    }

	sprite_draw(&food.sprite);

	show_screen();
}
/* Game States -------------------------------------------------------------------------*/

void startScreen () {
    clear_screen();
	char *name = "Waldo Fouche";
	char *sn = "n9950095"; 
	
	draw_string(0, 10, name, FG_COLOUR);
	draw_string(0, 20, sn, FG_COLOUR);
	draw_string(0, 30, "->SW2", FG_COLOUR);
	
	show_screen();	
	
	//PLAY THE GAME
	if ( BIT_IS_SET(PINF, 6) ) {
		while (BIT_IS_SET(PINF, 6)) {

        }

        clear_screen();
		game.start_screen = false;
		//clear_control_bits();
	}
}

void pauseMenu () {
    clear_screen();
	draw_string((LCD_X - 55) / 2, 0, "GAME PAUSED", FG_COLOUR);
	draw_string(0, 15, "SCORE: ", FG_COLOUR);
	draw_int(LCD_X - 10, 15, player.score);
	draw_string(0, 25, "LIVES: ", FG_COLOUR);
	draw_int(LCD_X - 10, 25, player.lives);
	draw_string(0, 35, "TIME:", FG_COLOUR);
	draw_time((LCD_X - 25), 35, timer.minutes,timer.seconds);
    gameControls();
	show_screen();
}

// End screen, when lives = 0, prompts user to restart game or display final score.
void gameOver () {
	clear_screen();
	draw_string((LCD_X - 55) / 2, 0, "GAME OVER!",FG_COLOUR);
	draw_string(0, 15, "-> SW3 Restart",FG_COLOUR);
	draw_string(0, 25, "-> SW2 show score",FG_COLOUR);
	game.over = true;

	if (BIT_IS_SET(PINF,5)) {
		while (BIT_IS_SET(PINF,5)) {
			// Block until button released.
		}
		game.over = false;
		timer.seconds = 0;
		setupGameWorld();
	} if (BIT_IS_SET(PINF,6)) {
		game.end_screen= true;
		game.over = false;
	}

	show_screen();
}

void endScreen () {
	clear_screen();
	draw_string((LCD_X - 55) / 2,0,"TOTAL SCORE",FG_COLOUR);
	draw_int((LCD_X - 55) / 2, 15,player.score);
	show_screen();
}
/* Game functions ----------------------------------------------------------------------*/
void playerPlatformCollision() {
	// Is the player falling?
	static bool falling = true; // If true, player is not on platform.
	static bool died = false; // Is the player dead?
	int current_platform = get_current_platform(&player.sprite);

	if (current_platform >=0) {
		if (platforms.safe[current_platform]) { // Is the current platform safe?
			if (falling) {
				player.score++;
			} falling = false;
				 movePlayer();
		}else died = true; // Player landed on forbidden platform
	} else {
		falling = true;
		player.sprite.y += player.sprite.dy;
	}

	int p_left = (int)round(player.sprite.x);
    int p_right = p_left + player.sprite.width - 1;
    int p_top = (int)round(player.sprite.y);
    int p_bottom = p_top+ player.sprite.height - 1;

    if (p_left < 0 || p_right >= LCD_X || p_bottom >=LCD_Y) {
        died = true;
    }

	if (died) {
		falling = false;
		player.lives --;
		
		if (player.lives > 1) {
			setupPlayer();
			spawnPlayer();
			died = false;
		}
		if (player.lives == 0) {

			gameOver();
		}
		
	}
}

void playerTreasureCollision() {
	if (sprites_collide(player.sprite,treasure.sprite)) {
		player.lives += 2;
		treasure.sprite.y = -200;
		spawnPlayer();
	}
}


void process () {
    gameControls();
	game_timer();
	moveTreasure();
	playerPlatformCollision();
	playerTreasureCollision();
	drawAll();  
}
/* MAIN --------------------------------------------------------------------------------*/


int main(void) {
	
	initializeTeensy();
	intialSetup();
	srand(game.seed*2317); //2317
    setupGameWorld();

	for ( ;; ) {
		if (game.start_screen) {
            
			startScreen();
		} else if (game.paused) {
			pauseMenu();

		} else if (game.over) {
			gameOver();
		} else if (game.end_screen) {
			endScreen();
		} else {
			game.seed++;
			process();   
		}	
	}
}
/* PROGRAM FINISH */

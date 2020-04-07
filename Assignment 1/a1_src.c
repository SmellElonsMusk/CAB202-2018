#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

/********************************
   Super Smash Bros
   By Waldo Fouche
   Student ID: n9950095
*********************************/

/* Program Start */
/* ------------------------------------------------------------------------*/

/* Configuration */
#define DELAY (10) /* Millisecond delay between game updates */
#define SEED ((unsigned)6785)
#define MAX_PLATFORM_WIDTH (10)
#define PLATFORM_HEIGHT (2)
#define PLAYER_WIDTH (5)
#define PLAYER_HEIGHT (4)
#define MAX_BLOCK_WIDTH (10)
#define MAX_SAFE_BLOCKS (160)
#define MAX_FORBIDEN_BLOCKS (40)

/* Game State */
// Boolean
bool game_over = false;    /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */
bool menu = true;
bool retry = true;
bool paused = false;

/* Global Variables */
int lives, play_time, score;
int num_safe, num_forbidden;
int current_dirn;
double start_time;

/* Sprites */
char *player_img =
    "  O  "
    " /\\ "
    "  |  "
    " / \\  ";

char *player_left_img =
    "  O  "
    " -|\\ "
    "  |  "
    " / \\  ";
char *player_right_img =
    "  O  "
    " /|- "
    "  |  "
    " / \\  ";

char *player_fall_img =
    "\\O/ "
    "  |  "
    "  |  "
    " / \\  ";

char *sblock_img =
    "=========="
    "==========";

char *fblock_img =
    "xxxxxxxxxx"
    "xxxxxxxxxx";

char *treasure_img =
    "&^"
    "&^";




/* Sprite id's */
sprite_id player;
sprite_id starting_block;
sprite_id safe_blocks[MAX_SAFE_BLOCKS];
sprite_id forbidden_blocks[MAX_FORBIDEN_BLOCKS];
sprite_id treasure;

/* Function Declarations*/
/* ------------------------------------------------------------------------*/
/* draws the main game HUD which displays the score, time and lives remaining */
void display_screen()
{
    int w, h, student_num;
    student_num = 9950095;
    get_screen_size(&w, &h);
    char b_symbol = '~';

    /* Create dispaly screen border */
    draw_line(0, 0, w - 1, 0, b_symbol);
    draw_line(0, 4, w - 1, 4, b_symbol);

    /* Create dispaly screen text */
    draw_formatted(5, 2, "Student Number: n%d            Lives Remaining: %d            Time: %d:%02d           Score: %d",
                   student_num, lives,(int)(get_current_time() - start_time) / 60, (int)(get_current_time() - start_time) % 60, score);

}

/* Creats the Hero, treasure and starting Platform */
void create_sprites()
{
    int w, h;
    w = 0;
    h = 0;
    get_screen_size(&w, &h);

    player = sprite_create(w - (MAX_PLATFORM_WIDTH / 2), 6, PLAYER_WIDTH, PLAYER_HEIGHT, player_img);
    starting_block = sprite_create(w - MAX_PLATFORM_WIDTH, 10, MAX_PLATFORM_WIDTH, PLATFORM_HEIGHT, sblock_img);
    treasure = sprite_create(rand() % rand() % ((screen_width() - 5 - 2) + 1), (h - 1) - 4, 2, 2, treasure_img);
}

/* Creates Blocks within a grid of squares deteremined by the size of the screen */
void create_blocks()
{
    /* Get Current Screen Size */
    int screen_width;
    int screen_height;
    get_screen_size(&screen_width, &screen_height);

    /* global Variables */
    num_forbidden = 0;
    num_safe = 0;

    /* Reset Current row and cols to 0 respectively*/
    int current_col = 0;
    int current_row = 0;

    int x_array[200];
    int y_array[200];

    /*Calculate Number of rows and columns based on screen size */
    int num_cols = screen_width / (MAX_PLATFORM_WIDTH + 1);
    int num_rows = (screen_height - 5) / (PLAYER_HEIGHT + PLATFORM_HEIGHT + 6);

    /* Initial x and y values */
    int x_val = 1;
    int y_val = 4 + (PLAYER_HEIGHT + PLATFORM_HEIGHT);

    /* Poplates the y_array with coordinates */
    for (int i = 0; i < num_rows; i++)
    {
        y_array[i] = y_val;
        y_val += (PLAYER_HEIGHT + PLATFORM_HEIGHT + 5);
    }

    /* Poplates the x_array with coordinates */
    for (int i = 0; i < num_cols; i++)
    {
        x_array[i] = x_val;
        x_val += (MAX_PLATFORM_WIDTH + 1);
    }

    /* Populate the Grids with Blocks vertically ( row by row ) */
    for (int i = 0; i < num_cols * num_rows; i++)
    {
        int random_number = rand() % 100 + 2;

        if (current_row >= num_rows)
        {
            current_col += 1;
            current_row = 0;
        }
        if (random_number > 0 && random_number <= 75)
        {
            safe_blocks[num_safe] = sprite_create(x_array[current_col], y_array[current_row], MAX_PLATFORM_WIDTH, PLATFORM_HEIGHT, sblock_img);
            num_safe += 1;
        }
        else if (random_number > 76)
        {
            forbidden_blocks[num_forbidden] = sprite_create(x_array[current_col], y_array[current_row], MAX_PLATFORM_WIDTH, PLATFORM_HEIGHT, fblock_img);
            num_forbidden += 1;
        }
        current_row += 1;
    }
}

/* Removes Random Safeblocks to create empty blocks */
void remove_blocks(void)
{
    int rand_i = 0;
    for (int i = 0; i <= num_safe; i++)
    {
        rand_i = rand() % 100 + 2;

        if (rand_i < 35)
        {
            //sprite_hide(safe_blocks[i]);
            sprite_move(safe_blocks[i], 1000, 0);
        }
    }
}

/* Draws all the platforms onto the screen created by create_blocks() */
void draw_platforms(void)
{
    for (int i = 0; i < num_forbidden - 1; i++)
    {
        sprite_draw(forbidden_blocks[i]);
    }
    for (int i = 0; i < num_safe - 1; i++)
    {
        sprite_draw(safe_blocks[i]);
    }
}

/* Reads Key pressed by the player */
int read_character()
{
    int keyCode = paused ? wait_char() : get_char();

    if ('p' == keyCode)
    {
        paused = !paused;
    }

    return keyCode;
}

void player_animation(char *image){
    int current_x = sprite_x(player);
        int current_y = sprite_y(player);
        sprite_destroy(player);
        player = sprite_create(current_x, current_y, PLAYER_WIDTH, PLAYER_HEIGHT, image);
}
/*Player Interactions */
void player_movement(int keyCode)
{
    int px = round(sprite_x(player));
    int py = round(sprite_y(player));

    if (keyCode == 'a' && px > 1)
    {
        player_animation(player_left_img);
        sprite_move(player, -1, 0);
    }
    else if (keyCode == 'd' && px < (screen_width() - 1 - sprite_width(player)))
    {
        player_animation(player_right_img);
        sprite_move(player, +1, 0);
    }
    /*
    else if (keyCode == 's' && py < (screen_height() - 1 - sprite_height(player)))
    {
        int current_x = sprite_x(player);
        int current_y = sprite_y(player);
        sprite_destroy(player);
        player = sprite_create(current_x, current_y, PLAYER_WIDTH, PLAYER_HEIGHT, player_fall_img);
        sprite_move(player, 0, +1);
    }
    */
    else if (keyCode == 'w' && py > 6)
    {
        player_animation(player_fall_img);
        sprite_move(player, 0, -1);
    }
}

/* Moves the treasure left to right within the game world */
void move_treasure()
{
    int dirn = rand() % 2;
    if (dirn == 0)
    {
        treasure->dx = -.05;
    }
    else
    {
        treasure->dx = .05;
    }
}

// Single Block Collisions
bool detect_block_collision(sprite_id sid1, sprite_id sid2)
{
    if ((!sid1->is_visible) || (!sid2->is_visible))
    {
        return false;
    }

    // collision box of sprite 1
    int left_s1 = round(sprite_x(sid1));
    int right_s1 = left_s1 + (sprite_width(sid1) - 1);
    int top_s1 = round(sprite_y(sid1));
    int bottom_s1 = top_s1 + (sprite_height(sid1));
    // Collision box for sprite 2
    int left_s2 = round(sprite_x(sid2));
    int right_s2 = left_s2 + (sprite_width(sid2) - 1);
    int top_s2 = round(sprite_y(sid2));
    int bottom_s2 = top_s2 + (sprite_height(sid2) - 1);

    if (left_s1 > right_s2)
    {
        return false;
    }
    if (left_s2 > right_s1)
    {
        return false;
    }
    if (top_s1 > bottom_s2)
    {
        return false;
    }
    if (top_s2 > bottom_s1)
    {
        return false;
    }

    // Everything else is a collsion
    return true;
}

// Collide with any
sprite_id detect_block_collision_any(sprite_id sid1, sprite_id sprites[], int n)
{
    sprite_id result = NULL;

    for (int i = 0; i < n; i++)
    {
        if (detect_block_collision(sid1, sprites[i]))
        {
            result = sprites[i];
            //sprite_destroy(sprites[i]);
        }
    }
    return result;
}

/* Draw Functions */
void draw_all()
{
    clear_screen();
    display_screen();
    sprite_draw(player);
    sprite_draw(starting_block);
    draw_platforms();
    sprite_draw(treasure);
    show_screen();
}

/* Other Game Functions  */
void respawn_player(void)
{
    sprite_destroy(player);
    create_sprites();
}

/* Game Over  */
void end_game(int keyCode)
{
    char *end_game_image =
    /**/ "              Game Over! You scored %d Points!            "
         "            Press r to restart and q to exit..            ";
    game_over = true;

    int msg_width = strlen(end_game_image) / 2;
    int msg_height = 2;
    int msg_x = (screen_width() - msg_width) / 2;
    int msg_y = (screen_height() - msg_height) / 2;
    sprite_id msg = sprite_create(msg_x, msg_y, msg_width, msg_height, end_game_image);
    clear_screen();
    sprite_draw(msg);
    show_screen();

    timer_pause(1000);

    while (get_char() >= 0)
    {
        // Do nothing
    }

    wait_char();
}

/* Program Main Function Declarations*/
/* ------------------------------------------------------------------------*/

/* Inital Game Setup */
void setup(void)
{
    lives = 10;
    start_time = get_current_time();
    srand(get_current_time());
    create_sprites();
    create_blocks();
    remove_blocks();
}

/* Main Game process to detect where player is and trigger events */
void process(void)
{
    int gravity = 1;
    int keyCode = read_character();
    player_movement(keyCode);
    move_treasure();

    if (detect_block_collision(player, starting_block))
    {
        gravity = 0;
        sprite_move(player, 0, gravity);
    }
    else if (detect_block_collision_any(player, safe_blocks, num_safe))
    {
        gravity = 0;

        score += 1;
        //player_animation(player_img);
        sprite_move(player, 0, gravity);

        //lives +=1;
    }
    else if (detect_block_collision_any(player, forbidden_blocks, num_forbidden))
    {
        lives -= 1;
        respawn_player();
    }
    else if (detect_block_collision(player, treasure))
    {
        lives += 2;
        respawn_player();
    }
    else
    {
        int current_x = sprite_x(player);
        int current_y = sprite_y(player);
        sprite_destroy(player);
        player = sprite_create(current_x, current_y, PLAYER_WIDTH, PLAYER_HEIGHT, player_fall_img);
        gravity = 1;
        
        sprite_move(player, 0, gravity);
    }

    /* Player Falls as a reuslt of gravity */

    if (sprite_y(player) > screen_height())
    {
        lives -= 1;
        respawn_player();
    }

    if (lives == 0)
    {
        end_game(keyCode);
    }
    else if (!player->is_visible)
    {
        end_game(keyCode);
    }

    if (gravity == 0)
    {
        //score+=1;
    }

    draw_all();
}

/* Clean up game */
void cleanup(void)
{
    // STATEMENTS
}

/* Program Entry point */
int main(void)
{
    /* code */
    score = 1;
    //setup_game();

    while (!game_over)
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
}
/* ------------------------------------------------------------------------*/
/* End Program */
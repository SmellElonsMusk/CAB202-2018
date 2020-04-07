#include <cab202_graphics.h>

void draw_bubbles(void){
	// (a) Draw a bubble at (76,14)
	draw_char( 76, 14, 'O');
	// (b) Draw a bubble at (79,8)
	draw_char( 79, 8, 'O');
	// (c) Draw a bubble at (67,5)
	draw_char( 67, 5, 'O');
	// (d) Draw a bubble at (53,8)
	draw_char( 53, 8, 'O');
	// Keep the following line without change.
	show_screen();
}

int main( void ) {
	setup_screen();

	draw_bubbles();

	draw_string( 0, screen_height() - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	return 0;
}
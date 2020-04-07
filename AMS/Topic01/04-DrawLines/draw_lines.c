#include <cab202_graphics.h>

void draw_lines(void){
	// (a) Draw a line from (9,3) to (74,21).
	draw_line(9,3,74,21,'^');

	// (b) Draw a line from (74,21) to (66,17).
	draw_line(74,21,66,17,'^');
	// (c) Draw a line from (66,17) to (15,6).
	draw_line(66,17,15,6,'^');
	// (d) Draw a line from (15,6) to (9,3).
	draw_line(15,6,9,3,'^');
	// Leave the following instruction unchanged.
	show_screen();
}

int main( void ) {
	setup_screen( );

	draw_lines();

	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char( );
	cleanup_screen( );
	return 0;
}
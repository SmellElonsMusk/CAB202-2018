#include "cab202_graphics.h"

void draw_platform(void) {
	// (a) Get the horizontal position of the left end of the platform
	//     using integer arithmetic and storing it in an integer value.
	int width = screen_width();
	// (b) Get the vertical position of the platform.
	int length = screen_height();
	// (c) Draw a horizontal line 9 units long starting at the position 
	//     you have calculated, using '^' symbols.
	int starting_pos = (width - length)/2;
	int h = screen_height();
	int vertical_pos = h-l-4;
	draw_line(starting_pos,vertical_pos, starting_pos+9, vertical_pos. '^');
	// Leave the following line intact.
	show_screen();
}


int main( void ) {
	setup_screen( );

	draw_platform( );

	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char( );
	cleanup_screen( );
	return 0;
}

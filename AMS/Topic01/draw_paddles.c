#include <cab202_graphics.h>
void draw_paddles( void ) {
	// find terminal height and width
	int w = screen_width(); 
	int h = screen_height();
	// left col
	int l = w + 4;
	// right col
	//int rc = w - 1;
	//int r = rc  - 2;
	
	// paddle at yt
	int yt1 =((h-6)/2) + 3;
	
	// paddle at yb 
	int yb1 = yt1 + 6 - 1;
	
	//draw_line(1,1,1,8,'$');
	draw_line(l,yt1,l,yb1,'$');
	//draw_line();
	
	
	show_screen( );
}
int main( void ) {
	setup_screen();
	draw_paddles();
	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	return 0;
}
#include <avr/io.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>
#include <graphics.h>
#include <lcd.h>
#include "lcd_model.h"

void setup( void ) {
	set_clock_speed(CPU_8MHz);

	//	(a) Enable input from the left button and right button.
    CLEAR_BIT(DDRB, 1);
    CLEAR_BIT(DDRB, 7);
	//	(b) Initialise the LCD display using the default contrast setting.

	//	(c) Use one of the functions declared in "graphics.h" to display 
	//	your student number, "n9950095", using the foreground colour, 
	//	positioning the left edge of the text at 18 and the nominal top 
	//	of the text at 18.

	//	(d) Use one of the functions declared in "graphics.h" to copy the contents 
	//	of the screen buffer to the LCD.
}

//	(e) Declare a global variable of type int named Contrast and 
//	initialise it to the default LCD contrast.

void process(void) {
	//	(f) Test pin corresponding to right button. If closed,
	//	decrement Contrast by 9. If the result is less than zero,
	//	set Contrast to the maximum permitted LCD contrast value.

	//	(g) Test pin corresponding to left button. If closed,
	//	increment Contrast by 9. If the result is greater
	//	then the maximum permitted LCD contrast value, set Contrast 
	//	to zero.

	//	(h) Send a sequence of commands to the LCD to enable extended
	//	instructions, set contrast to the current value of Contrast, 
	//	and finally retore the LCD to basic instruction mode.
}


int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(100);
	}

	return 0;
}

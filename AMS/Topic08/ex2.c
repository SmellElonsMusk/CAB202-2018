#include <avr/io.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <macros.h>
#include <graphics.h>
#include <lcd.h>
#include "lcd_model.h"

void setup( void ) {
	set_clock_speed(CPU_8MHz);

	//	(a) Enable input from the joystick-left switch and joystick-right switch.
	CLEAR_BIT(DDRB , 1);
	CLEAR_BIT(DDRD , 0);
	//	(b) Initialise the LCD display using the default contrast setting.
	lcd_init(LCD_DEFAULT_CONTRAST);
	//	(c) se one of the functions declared in "graphics.h" to display 
	//	your student number, "n9950095", using the foreground colour, 
	//	positioning the left edge of the text at 18 and the nominal top 
	//	of the text at 18.
	draw_string(18, 18, "n9950095", FG_COLOUR);
	//	(d) Use one of the functions declared in "graphics.h" to copy the contents 
	//	of the screen buffer to the LCD.
	show_screen();
}

//	(e) Declare a global variable of type int named lcd_contrast and 
//	initialise it to the default LCD contrast.
int lcd_contrast = LCD_DEFAULT_CONTRAST;

void process(void) {
	//	(f) Test pin corresponding to joystick-right switch. If closed,
	//	decrement lcd_contrast by 7. If the result is less than zero,
	//	set lcd_contrast to the maximum permitted LCD contrast value.
	if (BIT_IS_SET(PIND, 0)) {
		
		lcd_contrast = lcd_contrast - 7;
		
		if (lcd_contrast < 0b00000000) {
			lcd_contrast = 0b01111111;
		}
	}
	//	(g) Test pin corresponding to joystick-left switch. If closed,
	//	increment lcd_contrast by 7. If the result is greater
	//	then the maximum permitted LCD contrast value, set lcd_contrast 
	//	to zero.
	else if (BIT_IS_SET(PINB, 1)) {
		
		lcd_contrast = lcd_contrast + 7;
		
		if (lcd_contrast > 0b01111111) {
			lcd_contrast = 0b00000000;
		}
	}
	
	else {
		lcd_contrast = LCD_DEFAULT_CONTRAST;
	}
	//	(h) Send a sequence of commands to the LCD to enable extended
	//	instructions, set contrast to the current value of lcd_contrast, 
	//	and finally retore the LCD to basic instruction mode.
	LCD_CMD(lcd_set_function, lcd_instr_extended);
	LCD_CMD(lcd_set_contrast, lcd_contrast);
	LCD_CMD(lcd_set_function, lcd_instr_basic);
}


int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(100);
	}

	return 0;
}

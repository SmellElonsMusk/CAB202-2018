#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>

#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

//	(a) Implement a function called uart_initialise which requires no arguments and
//	returns no value.
void uart_initialise(){
    int rate = 19200;
	//	(a.a) Set the data transmission rate to 19200 bits per second,
	//	using the formula provided in the Topic 10 Technical Notes.
    UBRR1 = (F_CPU / 4 / rate - 1) / 2;
	//	(a.b) Set the UART speed to double speed.
    UCSR1A = (1 << U2X1);
	//	(a.c) Enable receive and transmit, but do not enable any UART-related
	//	interrupts.
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	//	(a.d) Set the character size to 7 bits, with no parity
	//	and 2 stop bits.
    UCSR1C = (1 << UCSZ11);
//	(b) End of function uart_initialise
}

int main(void) {
	uart_initialise();

	for ( ;; ) {}
}

#include <stdio.h>

void count_down( void ) {
	// (a) Print the introductory message.
	printf("Reverse order list of integers greater than 22, beginning at 341, stepping by -5.\n");
	// (b) Declare an integer variable that you will use to count. Initially 
	//     the counter should equal the start value, 341.
	int count = 341;
	while (count >=22){
	// (c) Begin a WHILE statement that will execute its loop body if the 
	//     counter is greater than the end value, 22.
	
		// (e) Print the value of the counter on a line by itself.
		printf("%d\n", count);
		count = count -5;
		// (f) Subtract the step size, 5, from the counter.
	// (d) End the WHILE statement
	}
}

int main( void ) {
	count_down();
	return 0;
}
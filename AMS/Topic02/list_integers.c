#include <stdio.h>

void list_integers( void ) {
	// (a) Print the introductory message.
	printf("Listing integers between 58 and 367...\n");
	// (b) Declare an integer variable that will be used as a counter. 
	//     Make it initially equal to the start value.
	int counter = 58;
	// (c) Begin a WHILE statement that will execute its loop body if the
	//     counter is less than or equal to the end value.
	while (counter <=367){
		printf("%d\n", counter);
		counter = counter + 1;
	
		// (e) Print the current value of the counter variable.

		// (f) Add 1 to the counter variable.

	// (d) End the WHILE statement that starts at (c).
	};
}

int main( void ) {
	list_integers();
	return 0;
}
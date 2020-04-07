#include <stdio.h>
#include <stdlib.h>

#define TRACE \
    ;         \
    fprintf(stderr, "%s: %d\n", __FILE__, __LINE__);

void print_stuff(int xxx, int *ppp)
{
    printf("x = %D\n", xxx);
    printf("x_addr = %p\n", ppp);
    printf("x_addr is also %ld\n", (long)ppp);
}

// Pointers Demo

int main(int argc, char **argv)
{
    /* code */
    int x = 123456;
    int *x_addr = &x;

    printf("x = %D\n", x);
    printf("x_addr = %p\n", x_addr);

    printf("x_addr is also %ld\n", (long)x_addr);

    *x_addr = 987654321;
    // TRACE;;

    print_stuff(x,x_addr);

    printf("x = %D\n", x);
    // TRACE;;
    printf("x_addr = %p\n", x_addr);
    // TRACE;;
    printf("x_addr is also %ld\n", (long)x_addr);
    //// // TRACE;;;

    x_addr = (int *)987654321;
    //// TRACE;;

    // *x_addr = 987654321;

    printf("x = %D\n", x);
    // TRACE;;

    fprintf(stderr, "x_addr = %p\n", x_addr);
    // TRACE;;
    fprintf(stderr, "x_addr = %d\n", *x_addr);
    printf("x_addr is also %ld\n", (long)x_addr);

    return 0;
};

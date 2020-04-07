#include <stdio.h>

//  (a) Begin the definition a function called Minimum that returns the
//      minimum of its three double precision floating point 
//      arguments.
// 
//      For all triples of double precision floating point values (x, y, z) the 
//      function must satisfy the following conditions:
//          ((Minimum(x,y,z) = x) OR (Minimum(x,y,z) = y OR (Minimum(x,y,z) = z))
//      AND (Minimum(x,y,z) <= x)
//      AND (Minimum(x,y,z) <= y)
//      AND (Minimum(x,y,z) <= z)

double Minimum (double x, double y, double z)
{
    //  (b) Implement the logic required to calculate the minimum
    //      of the three input values, and return the result.
    if (x <= y && x <= z) return x;
	
	else if (y <= x && y <= z) return y;
	
	else return z;

}

int main(void) {
    double x, y, z, minVal;

    printf("Please enter three numeric values: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    minVal = Minimum(x, y, z);
    printf("Minimum(%0.10f, %0.10f, %0.10f) = %0.10f\n", x, y, z, minVal);

    return 0;
}

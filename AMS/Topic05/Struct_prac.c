#include <stdio.h>
#include <stdbool.h>

//  (Task 1) Complete the declaration of a new struct type called location_t. 
//  This structure could be used to record a position in a two-dimensional
//  discrete coordinate system.
// 
//  Members:
//      x_loc - an int which records the horizontal position of the 
//                     point.
//      y_loc - an int which records the vertical position of the 
//                     point.

typedef struct location_t
{
    //  (Task 1.1) Declare a field called x_loc of type int.
    int x_loc;

    //  (Task 1.2) Declare a field called y_loc of type int.
    int y_loc;

} location_t;

//  (Task 2) Define a function called location_read which uses printf and 
//  scanf to get the user to enter a location_t coordinate pair. 
// 
//  Parameters:
//      loc - the address of a location_t which must be populated by the 
//      function call.
//
//  Returns:
//      The function must return a boolean value indicating the status of the 
//      I/O operation. The status is true if and only if two integer values 
//      have been successfully parsed and saved in loc.

void location_read (void)
{
    //  (Task 2.1) Declare an integer called items_read with initial value 0.
    int items_read = 0;
    //  (Task 2.2) Display prompt "Please enter x_loc: ".
    printf("Please enter x_loc: ");
    //  (Task 2.3) Use scanf to try to read an integer value using scanf into
    //  the loc->x_loc field of the struct pointed at by loc. Add 
    //  the value returned by scanf to items_read.
    scanf(*loc_>x);
    //  (Task 2.4) Display prompt "Please enter y_loc: ".
    
    //  (Task 2.5) Use scanf to try to read an integer value into the y_loc 
    //  field of the struct pointed at by loc.
    //  Add the value returned by scanf to items_read.
    
    //  (Task 2.6) Return true if and only if items_read is equal to 2.
}

//  (Task 3) Define a function called location_write which uses printf to 
//  display the value of a location_t structure. 
// 
//  Parameters:
//      loc - a location_t structure that will be displayed.
//
//  Returns:
//      Nothing.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    //  (Task 3.1) Print the horizontal and vertical position of SCREEN-POS_VAR
    //  with format string "{%d, %d}". Do NOT insert a linefeed.
}

//  (Task 4) Define a function called location_dist which calculates
//  the city block distance between two location_t objects.
//  Ref: https://en.wikipedia.org/wiki/Taxicab_geometry
// 
//  Parameters:
//      loc_1 - a location_t structure.
//      loc_2 - a location_t structure.
//
//  Returns:
//      An int which is equal to the city block distance between the points
//      represented by the arguments.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    //  (Task 4.1) Compute and return the city block distance between the points 
    //  represented by loc_1 and loc_2.
}

//  (Task 5) Define a function called location_nearest which finds the
//  location_t object in a list which is closest to a query point, using
//  city block distance.
// 
//  Parameters:
//      query_loc - a location_t structure.
//      locations - an array of location_t structures.
//      num_locations - an int which tells the function how many elements there 
//      are in the array.
//
//  Returns:
//      A pointer to a location_t object. If num_locations is equal to or less 
//      than 0 this value will be NULL. Otherwise, it will be the address
//      of the location_t object in the array that is nearest to the query 
//      point. If multiple candidates share the minimum distance to the query 
//      point, return the address of the first object in the list for which 
//      this is so.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    //  (Task 5.1) If num_locations equal to or less than 0, return NULL.

    //  (Task 5.2) Declare and initialise a pointer to location_t called nearest.
    //  The initial value is the address of the first element in the array.

    //  (Task 5.3) Declare and initialise an integer called min_dist.
    //  The initial value is the city block distance from the query to
    //  the first element of the array.
    //  Hint: use location_dist.

    //  (Task 5.4) Set up a for loop to iterate over the array.
    //  Skip the first element of the array, because we already know
    //  the distance from the first element to the query.
    FOR ( SOMETHING SOMETHING SOMETHING )
    {
        //  (Task 5.4.1) Compute the city block distance from the query
        //  to the current element of the array. This is the current
        //  distance. Make sure you remember it somehow.

        //  (Task 5.4.2) If the current distance is less than min_dist:
        IF ( SOMETHING )
        {
            //  (Task 5.4.3) Overwrite min_dist with the current distance.
            //  Overwrite nearest with the address of the current element of
            //  the array.
        }
    }

    //  (Task 5.5) Return nearest.
}


#define MAX_ITEMS (100)

int main(void)
{
    location_t query;
    printf("Input query point:\n");
    location_read(&query);

    location_t ref_points[MAX_ITEMS] = { {0,0} };
    int num_items;

    // Get number of ref_points.
    printf("Please enter number of items (up to %d) that will be processed: ", MAX_ITEMS);
    scanf("%d", &num_items);

    // if number of ref_points exceeds array size, restrict it to that value.
    if (num_items > MAX_ITEMS)
    {
        num_items = MAX_ITEMS;
    }

    for (int i = 0; i < num_items; i++)
    {
        printf("Please enter item %d of %d:\n", (i + 1), num_items);
        location_read(&ref_points[i]);
    }

    for (int i = 0; i < num_items; i++)
    {
        printf("Distance from ");
        location_write(ref_points[i]);
        printf(" to ");
        location_write(query);
        printf(" is %d\n", location_dist(ref_points[i], query));
    }

    location_t * nearest = location_nearest(query, ref_points, num_items);

    if (nearest)
    {
        printf("The closest point is ");
        location_write(*nearest);
        printf(", at a distance of %d.\n", location_dist(query, *nearest));
    }
    else
    {
        printf("Collection is empty.\n");
    }

    return 0;
}

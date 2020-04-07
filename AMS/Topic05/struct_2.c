#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//  (Task 1) Declare a new struct type called date_t. This structure 
//  could be used to store a calendar date in an application which requires 
//  timekeeping on a daily basis.
// 
//  Members:
//      year - an int which records the year.
//
//      month - an int which records the month, with valid values ranging 
//      from 1 to 12.
//
//      day - an int which records the day in the month, ranging from 1 to
//      31, subject to the rules set out in the documentation comment of the 
//      date_valid function.

typedef struct date_t
{
        //  (Task 1.1) Declare the fields of the struct in the order listed above.
    int year;
    int month [12];
    int day;
} date_t;

//  (Task 2) Define a function called date_read which uses scanf to get the 
//  data for a date_t. Fields are to be entered as three separate int values
//  in the format "%d-%d-%d".
// 
//  Parameters:
//      date_ptr - the address of a date_t which must be populated by the 
//      function call.
//
//  Returns:
//      The function must return a boolean value indicating the status of the 
//      I/O operation. The status is true if and only if three integer values 
//      have been successfully parsed and saved in date_ptr.
//
//      Do not try to perform other data validation in this function.

void date_read (void)
{
    //  (Task 2.1) Insert logic to read three integer values from standard input
    //  and save them in the appropriate fields of date_ptr. Use scanf, but do
    //  NOT use printf or any other output function. 
}

//  (Task 3) Define a function called date_write which uses printf to 
//  display the value of a date_t structure. 
// 
//  Parameters:
//      date - a date_t structure that will be displayed.
//
//  Returns:
//      Nothing.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    //  (Task 3.1) Print the horizontal and vertical position of SCREEN-POS_VAR
    //  with format string "%d-%d-%d". Do NOT insert a linefeed.
}

//  (Task 4) Define a function called date_compare which compares two
//  date_t values. Your implementation may assume that these values are 
//  valid dates.
// 
//  Parameters:
//      date1 - a date_t structure.
//      date2 - a date_t structure.
//
//  Returns:
//      An int which is:
//      -1  if the date represented by date1 is before that represented by 
//          date2;
//      0   if the two values represent the same date;
//      +1 otherwise.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    INSERT CODE HERE
}
 
//  (Task 5) Implement the date_valid function which determines if the 
//  supplied date is valid:
//  *   Year must be greater than or equal to 1.
//  *   Month must between 1 and 12 inclusive.
//  *   Day must be at least 1, with upper limits given below:
//      30 days: September, April June, and November.
//      31 days: January, March, May, July, August, October, December.
//      28 or 29: February (usually 28, but 29 in a leap year).
//  
//  A year is a leap year if it is divisible by 400, or if it is
//  divisible by 4 but not divisible by 100.
//  
//  Parameters:
//      date - a date_t value.
//
//  Returns:
//      Returns true if and only if the supplied date is valid according to 
//      the definition listed above.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    INSERT CODE HERE
}

//  (Task 6) Define a function called date_match which compares a query date to
//  the elements of an array of date_t objects. The function returns the 
//  address of the first object in the list which satisfies a designated criterion.
// 
//  Parameters:
//      query - a date_t structure.
//      dates - an array of date_t structures.
//      num_dates - an int which tells the function how many elements there 
//          are in the array.
//      criterion - an int (guaranteed to be -1, 0, or 1) which defines the 
//          matching criterion. 
//
//  Returns:
//      A pointer to a date_t object.
//      If num_dates is equal to or less than 0: this value will be NULL.
//      If the query is not valid: this value will be NULL.
//      If there is no valid element x in the array which
//              date_compare(x,query) == criterion
//          then this value will be NULL.
//      Otherwise: the return value will be the address of the first valid 
//          date_t x in the array for which 
//              date_compare(x,query) == criterion.

INSERT RETURN_TYPE FUNCTION_NAME ( PARAMETER_LIST )
{
    INSERT CODE HERE
}


#define MAX_ITEMS (100)

int main(void)
{
    date_t query;
    printf("Input query date using format %s, with year first and day last: ", "%d-%d-%d");
    date_read(&query);

    date_t ref_dates[MAX_ITEMS] = { {0,0,0} };
    int num_items;

    // Get number of ref_dates.
    printf("Please enter number of items (up to %d) that will be processed: ", MAX_ITEMS);
    scanf("%d", &num_items);

    // if number of ref_dates exceeds array size, restrict it to that value.
    if (num_items > MAX_ITEMS)
    {
        num_items = MAX_ITEMS;
    }

    for (int i = 0; i < num_items; i++)
    {
        printf("Please enter item %d of %d using format %s, with year first and day last: ", (i + 1), num_items, "%d-%d-%d");
        date_read(&ref_dates[i]);
    }

    for (int i = 0; i < num_items; i++)
    {
        date_write(ref_dates[i]);

        if (!date_valid(ref_dates[i]))
        {
            printf(" is not valid.\n");
            continue;
        }

        int cmp = date_compare(ref_dates[i], query);
        if (cmp < -1 || cmp > 1)
        {
            printf("Error!!! date_compare is broken.\n");
            exit(1);
        }
        char * labels[] = { "less than", "equal to", "greater than" };
        printf(" is %s ", labels[cmp + 1]);
        date_write(query);
        printf("\n");
    }

    const int criterion = -1;
    date_t * cmp = date_match(query, ref_dates, num_items, criterion);

    if (cmp)
    {
        printf("The first valid date matching the search criterion is ");
        date_write(*cmp);
    }
    else
    {
        printf("There is no valid date matching the search criterion.\n");
    }

    return 0;
}

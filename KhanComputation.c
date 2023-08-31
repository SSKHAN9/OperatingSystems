/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <math.h>

int main(void)
{
	//Add a prompt for the number of cylinders to sum. Make sure to use the control
	//symbol for integers. [2 points]

    int num;
    float height, radius, volume, totalVolume = 0;

    printf("Enter the number of cylinders to sum: ");
    scanf("%d", &num);


	//Create a loop based on the number of cylinders the user enters. [2 points]

	//    Within the loop, prompt for height and radius parameters (allow floating
	//    point numbers), and calculate the volume for that particular cylinder.
	//    [4 points]

    for (int i = 0; i < num; i++) {
        printf("Enter the height of cylinder %d: ", i + 1);
        scanf("%f", &height);

        printf("Enter the radius of cylinder %d: ", i + 1);
        scanf("%f", &radius);

        volume = M_PI * radius * radius * height;
        totalVolume += volume;
    }

	//Display the total volume sum back to the user. Make sure to use the right control
	//symbol. [2 points]

    printf("The total volume of the cylinders is: %f", totalVolume);

	return 0;
}
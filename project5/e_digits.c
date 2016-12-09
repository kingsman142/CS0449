#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "e.h"

int main(int argc, char* argv[]){
	//User is not allowed to pass in any number of arguments other than 2.
	//The number 3 is in the conditional because argument 0 is the executed file.
	if(argc != 3){
		printf("Exactly two arguments are required! Number of arguments passed in: %d\n", argc-1);
		return 1;
	}

	//Convert the string command-line arguments to integers
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	if((x == 0 && strcmp(argv[1], "0") != 0) || (y == 0 && strcmp(argv[2], "0") != 0)){ //There was an error parsing the numbers
		printf("Arguments must be integers!\n");
		return 1;
	}

	//The second index cannot be less than the first, otherwise num_digits will be negative
	if(y < x){
		printf("Second argument is less than the first!\n");
		return 1;
	}

	//If either index is negative, that should be completely invalid input.
	if(y < 0 || x < 0){
		printf("At least one of the arguments is negative!\n");
		return 1;
	}

	int num_digits = y + 1; //Calculate the number of digits needed.
				//If y and x are the same value (1 and 1), then the number of digits should be 2.

	char e_buffer[num_digits]; //Create the buffer that will store the digits of e
	int e_driver = open("/dev/e", O_RDONLY); //Read from the e device driver
	read(e_driver, e_buffer, y+3); //Get the digits of e and place them into the buffer.
			  	       //Due to the algorithm, the argument m has to be 3 more than num_digits or indices 5 and 12 won't work.

	//Print out the digits
	int i = 0;
	for(i = x; i <= y; i++){
		printf("%c", e_buffer[i]);
	}
	printf("\n");
}

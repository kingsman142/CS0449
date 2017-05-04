#include <stdio.h>
#include <string.h>

#define array_length(array) sizeof(array)/sizeof(array[0])

int main(){
    srand((unsigned int) time(NULL));
	
    //Necessary data for the game
    char * colors[] = {"Red", "Orange", "Yellow", "Green", "Blue", "Purple"};
    char color_keys[] = {'r', 'o', 'y', 'g', 'b', 'p'};
    char pegs[4];
    char user_guess[5];
    char play_again[1];
    int i;

    printf("Welcome to Mastermind!\n\n");	

    //Initial prompt
    printf("Would you like to begin playing (yes or no)? ");
    scanf("%s", play_again);

    //Game instructions
    if(strcmp(play_again, "yes") == 0){
        printf("\nKeys & Rules\n-----------------------\n");
        printf("r = Red\n");
        printf("o = Orange\n");
        printf("y = Yellow\n");
        printf("g = Green\n");
        printf("b = Blue\n");
        printf("p = Purple\n");
        printf("Input must consist of only 4 colors and they must be connected together.\n");
        printf("\tFor example, ropp = Red Orange Purple Purple.\n");
        printf("You are allowed 10 guesses until you lose.\n");
    }

    //While the user keeps on saying "yes", play the game
    while(strcmp(play_again, "yes") == 0){
        int j;
	int k;
        int turns_played = 0;
        int colors_in_correct_place = 0;
        int colors_correct_but_wrong_place = 0;

	//Generate 4 random pegs for this game round
        for(i = 0; i < array_length(pegs); i++){
            pegs[i] = color_keys[rand() % 6];
        }
	
	//The game conditions are that the user has played less t han 10 turns
	//	or they have not guessed all 4 colors yet
        while(turns_played < 10 && colors_in_correct_place < 4){
	    int colors_marked[4] = {0, 0, 0, 0}; //Keep track of whether or not they guessed each color
            colors_in_correct_place = 0; 
            colors_correct_but_wrong_place = 0;

            printf("\nEnter your guess: ");
            scanf("%s", user_guess);
	    
	    //Search user input and determine which pegs are in the correct place
            for(j = 0; j < 4; j++){
                if(user_guess[j] == pegs[j]){
		    colors_in_correct_place++;
		    colors_marked[j] = 1;
		}
            }
	    
	    //Search user input and determine which pegs are the correct color but in the wrong place
	    for(j = 0; j < 4; j++){
		for(k = 0; k < array_length(pegs); k++){
		    if(colors_marked[k] == 0 && user_guess[j] == pegs[k]){
			colors_marked[k] = 1;
			colors_correct_but_wrong_place++;
			break;
		    }
		}
	    }

	    //Output statistics to user for each turn
            printf("\nColors in the correct place: %d\n", colors_in_correct_place);
            printf("Colors correct but in wrong position: %d\n", colors_correct_but_wrong_place);

            turns_played++;
        }

	//Win and lose messages after each game round has been played.
	//If the user lost, output what the correct answer was.
        if(colors_in_correct_place == 4){
	    printf("\nCongratulations! You won!\n");
	} else{
            printf("\nYou lost because you used up all 10 turns!\n");
            printf("The correct answer was %s.\n", pegs);
        }

	//Ask the user if they want to play again to keep the while condition going or not.
        printf("\nWould you like to play again? ");
        scanf("%s", play_again);
    }

    return 0;
}



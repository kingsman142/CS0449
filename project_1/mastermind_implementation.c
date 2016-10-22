#include <stdio.h>
#include <string.h>

#define array_length(array) sizeof(array)/sizeof(array[0])

int main(){
    srand((unsigned int) time(NULL));

    char * colors[] = {"Red", "Orange", "Yellow", "Green", "Blue", "Purple"};
    char color_keys[] = {'r', 'o', 'y', 'g', 'b', 'p'};
    char pegs[] = "cat";
    char user_guess[5];
    char play_again[1];
    int i;

    printf("Welcome to Mastermind!\n\n");

    printf("Would you like to begin playing (yes or no)? ");
    scanf("%s", play_again);

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
    while(strcmp(play_again, "yes") == 0){
        int j;
        int turns_played = 0;
        int colors_in_correct_place = 0;
        int colors_correct_but_wrong_place = 0;

        for(i = 0; i < array_length(pegs); i++){
            //pegs[i] = color_keys[rand() % 6];
            //printf("Peg %d: %c\n", i, pegs[i]);
        }
        printf("Word: %s\n\n", pegs);

        while(turns_played < 10 && colors_in_correct_place < 4){
            colors_in_correct_place = 0;
            colors_correct_but_wrong_place = 0;

            printf("\nEnter your guess: ");
            scanf("%s", user_guess);

            for(j = 0; j < 4; j++){
                if(user_guess[j] == pegs[j]) colors_in_correct_place++;
            }

            printf("\nColors in the correct place: %d\n", colors_in_correct_place);
            printf("Colors correct but in wrong position: %d\n", colors_correct_but_wrong_place);

            turns_played++;
        }

        if(turns_played == 10){
            printf("\nYou lost because you used up all 10 turns!\n");
            printf("The correct answer was %s.\n", *pegs);
        } else{
            printf("\nCongratulations! You won!\n");
        }

        printf("\nWould you like to play again? ");
        scanf("%s", play_again);
    }

    return 0;
}

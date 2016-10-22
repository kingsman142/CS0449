#include <stdio.h>

int main(){
	printf("Please enter a weight (in pounds) you'd like to convert: ");
	float weight;
	scanf("%f", &weight);
	printf("\nHere are the weights on other planets: \n\n");
	printf("Mercury: \t%d lbs\n", (int) (weight*.38));
	printf("Venus: \t\t%d lbs\n", (int) (weight*.91));
	printf("Mars: \t\t%d lbs\n", (int) (weight*.38));
	printf("Jupiter: \t%d lbs\n", (int) (weight*2.54));
	printf("Saturn: \t%d lbs\n", (int) (weight*1.08));
	printf("Uranus: \t%d lbs\n", (int) (weight*.91));
	printf("Neptune: \t%d lbs\n", (int) (weight*1.19));
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	int grade;
	struct Node* next;
};

int main(){
	struct Node* head = NULL;
	struct Node* curr = head;

	while(1){
		int input_grade;
		
		printf("Enter a grade: ");
		scanf("%d", &input_grade);

		if(input_grade != -1){
			if(head == NULL){
				head = malloc(sizeof(struct Node));
				head->grade = input_grade;
				curr = head;
			} else{
				struct Node* new_node = malloc(sizeof(struct Node));
				new_node->grade = input_grade;
				curr->next = new_node;
				curr = new_node;
			}
		} else{
			break;
		}
	}

	curr = head;
	int numElements = 0;
	int total = 0;

	while(curr != NULL){
		total += curr->grade;
		numElements++;
		curr = curr->next;
	}
	
	curr = head;
	while(curr != NULL){
		struct Node* nextReference = curr->next;
		free(curr);
		curr = nextReference;
	}

	printf("The average was: %.2f\n", ((double) total)/numElements);

	return 0;
}

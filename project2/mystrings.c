//James Hahn
//CS0449, M/W 3-4:15pm
//Recitation: Tu 12-1pm

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Appropriate number of command-line arguments not passed in.  One is required.\n");
		return 1;
	}

	FILE* file_to_read;
	file_to_read = fopen(argv[1], "rb");
	
	if(file_to_read == NULL) printf("Failed to load file\n");
	
	char* current_string = malloc(100);
	unsigned char next_char;

	while(fgetc(file_to_read) != EOF){
		fseek(file_to_read, -1, SEEK_CUR);
		next_char = fgetc(file_to_read);

		if(next_char >= 32 && next_char <= 126){
			if(*current_string == '\0'){
				*current_string = next_char;
			} else{ 
				strncat(current_string, &next_char, 1);
			}
		} else if(*current_string != '\0'){
			if(strlen(current_string) >= 4){
				printf("%s\n", current_string);
			}
			memset(current_string, 0, 100);
		}
	}

	fclose(file_to_read);
}

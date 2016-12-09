#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* cmd_args[500];
int number_of_args = 0;

//Tokenize the entire input command to easily manipulate later on
void tokenize_command(char* command){
	char* delims = " \t\n()<>|&;"; //List of delimiters

	char* token = strtok(command, delims);
	cmd_args[number_of_args++] = token;

	while(token != NULL){ //Tokenize the command and place all tokens into an array
		token = strtok(NULL, delims);
		cmd_args[number_of_args++] = token;
	}

	number_of_args--; //When there's a NULL token, the number of arguments is increased to 1 too many
}

//Change the current working directory
void change_directory(){
	char* path = cmd_args[1];

	if(path == NULL){
		printf("\t! Please specify a directory !\n");
	} else{
		int success_or_failure = chdir(path);

		if(success_or_failure == -1){
			printf("\t! Invalid directory !\n");
		}
	}
}

//If the command isn't "exit" or "cd", execute every other unix command
int call_unix_command(char* full_command){	
	int status = 0;
	int new_process_id = fork(); //Create a new process to execute the new command

	if(new_process_id != 0){ //Wait for process to change to child process
		wait(&status);
	} else{ 
		FILE* io_file = NULL;

		char* pointer_to_append_arrows = strstr(full_command, ">>");
		char* pointer_to_write_arrow = strstr(full_command, ">");
		char* pointer_to_input_redirect = strstr(full_command, "<");

		if(pointer_to_append_arrows != NULL){ //>> arrows mean append output to a file
			char* next_token = strtok(pointer_to_append_arrows, " \t\n()<>|&;");
			io_file = freopen(next_token, "a", stdout);

			if(io_file == NULL){
				printf("\t! File failed to open ! Execution failed !\n");
				exit(1);
			}
			
			cmd_args[number_of_args-- - 1] = NULL; //Nullify the last argument so it's not passed into execvp
		} else if(pointer_to_write_arrow != NULL){ //> arrow means write output to a file
			char* next_token = strtok(pointer_to_write_arrow, " \t\n()<>|&;");
			io_file = freopen(next_token, "w", stdout);

			if(io_file == NULL){
				printf("\t! File failed to open ! Execution failed !\n");
				exit(1);
			}

			cmd_args[number_of_args-- - 1] = NULL; //Nullify the last argument so it's not passed into execvp
		}
		if(pointer_to_input_redirect != NULL){ //< arrow means feed contents of a file to the arguments of a command
			char* next_token = strtok(pointer_to_input_redirect, " \t\n()<>|&;");
			io_file = freopen(next_token, "r", stdin);

			if(io_file == NULL){
				printf("\t! File failed to open ! Execution failed !\n");
			}

			cmd_args[number_of_args-1] = NULL; //Nullify the last argument so it's not passed into execvp
		}

		int exec_success = execvp(cmd_args[0], cmd_args); //Execute the command with its arguments
		if(exec_success == -1){
			printf("\t! Invalid command !\n");
			exit(1);
		}

		exit(0); //Successful exit of child process
	}
	
	return 0;
}

void run_shell(){
	char command[500];
	
	while(1){
		printf("shell : "); //Increase the user experience!

		fgets(command, sizeof(command), stdin);
		if(command == NULL || strlen(command) == 1){
			printf("\t! Please enter a command !\n");
			continue;
		}

		int command_length = strlen(command);
		command[command_length-1] = 0; //Clear the newline character from the command
		
		char command_copy_for_tokenization[strlen(command)];
		strcpy(command_copy_for_tokenization, command); //Copy command into string placeholder

		tokenize_command(command_copy_for_tokenization); //Tokenize the input
		
		if(!strcmp(cmd_args[0], "exit")){ //Exit
			exit(0);
		} else if(!strcmp(cmd_args[0], "cd")){ //Change directory
			change_directory();
		} else{ //Execute every other command
			call_unix_command(command);			
		}

		number_of_args = 0; //Reset the number of arguments for the next command
	}
	
	exit(1); //Improper exit
}

int main(){
	run_shell();
	return 0;
}

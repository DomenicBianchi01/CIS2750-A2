/*Domenic Bianchi
CIS 2750 Assignment 2
February 19, 2017
This program adds or removes a user from a stream or multiple streams*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "addauthor.h"
#include "stream.h"

int main(int argc, char * argv[]) {

	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-r") == 0)) {

		printf("%s\n", "Username not included.");
		return 1;
	}

	char list[256];
	char name[256];
	int i = 1;
	bool removeFlag = false;
	clearArray(list, 256);
	clearArray(name, 256);

	/*Loop through all arguments looking for the arguments that make up the username and if a -r flag is present*/
	for (i = 1; argv[i] != NULL; i++) {

		if (strcmp(argv[i], "-r") != 0) {

			/*Add argument to username string*/
			strcat(name, argv[i]);

			/*Multiple username arguments mean the username is multiple words so seperarte the arguments/words by a space*/
			if (argv[i+1] != NULL) {

				strcat(name, " ");
			}
		}
		/* -r flag found*/
		else {

			removeFlag = true;
		}
	}

	promptForStreams(list);
	updateStreams(name, list, removeFlag);

	return 0;
}

void promptForStreams(char streamList[]) {

	printf("list streams: ");
	/*Prompt user for a list of streams to "subscribe" to and add a null terminator at the end of the string*/
	fgets(streamList, 256, stdin);
	streamList[strlen(streamList)-1] = '\0';
}

void updateStreams(char username[], char streamList[], bool flag) {

	/*If falg is true, that means the -r flag was included in the user input and the user should be removed for the stream(s)*/
	if (flag == true) {

		removeUser(username, streamList);
	}
	else {

		addUser(username, streamList);
	}
}

void clearArray(char string[], int length) {

	int j = 0;

	/*Set entire array to null terminators*/
	for (j = 0; j < length; j++) {
		
		string[j] = '\0';
	}
}
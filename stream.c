/*Domenic Bianchi
CIS 2750 Assignment 2
February 19, 2017
This program creates files neccessary to create a stream, add a user to a user file, or remove a user from a user file. Also, this program adds posts to the appropriate stream*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stream.h"
#include "addauthor.h"

void updateStream(userPost * st) {

	FILE * dataFile = NULL;

	int byteCount = 0;
	bool foundUser = false;
	char fileName[256];
	char lineString[256];
	char name[256];
	clearArray(fileName, 256);
	clearArray(lineString, 256);
	clearArray(name, 256);

	strcpy(fileName, "./messages/");
	strcat(fileName, st->streamname);
	strcat(fileName, "StreamUsers.txt");

	dataFile = fopen(fileName, "r");

	if (dataFile == NULL) {

		printf("Stream does not exist\n");
		return;
	}
	else {

		/*Search though all the usernames that have access to the stream and check if the user trying to post to the stream is allowed to*/
		while (fgets(lineString, 256, dataFile) != NULL) {

			int i = 0;

			/*Starting from the end of the string, search for the first space. Everything before the space is the username*/
			for (i = strlen(lineString)-1; i >= 0; i--) {

				if (lineString[i] == ' ') {

					strncpy(name, lineString, i);
					name[i] = '\0';
					break;
				}
			}

			/*If the username from the file matches the user trying to post, then the user can post*/
			if (strcmp(name, st->username) == 0) {

				foundUser = true;
				break;
			}
		}

		fclose(dataFile);
	}

	if (foundUser == false) {

		printf("User does not have permission to post in this stream\n");
		return;
	}

	/*Add the post to the stream*/
	strcpy(fileName, "./messages/");
	strcat(fileName, st->streamname);
	strcat(fileName, "Stream.txt");

	dataFile = fopen(fileName, "a");
	fprintf(dataFile, "Sender: %s\nDate: %s%s", st->username, st->date, st->text);
	fclose(dataFile);
	clearArray(fileName, 256);

	/*Add the byte index to the stream data file*/
	strcpy(fileName, "./messages/");
	strcat(fileName, st->streamname);
	strcat(fileName, "StreamData.txt");

	dataFile = fopen(fileName, "r");

	/*Calculate number of bytes for the post*/
	while (fgets(lineString, 256, dataFile) != NULL) {

		byteCount = atoi(lineString);
	}

	byteCount = byteCount + strlen(st->username) + strlen(st->date) + strlen(st->text) + 15;

	fclose(dataFile);
	dataFile = fopen(fileName, "a");
	fprintf(dataFile, "%d\n", byteCount);
	fclose(dataFile);
}

void addUser(char * username, char * list) {

	FILE * dataFile = NULL;

	char * streamToAdd;
	char temp[256];
	char name[256];
	char lineString[256];

	/*Tokenize all streams that the user is being added to*/
	streamToAdd = strtok(list,",");

	while (streamToAdd != NULL) {

		bool addUserToStream = true;
		clearArray(temp, 256);
		clearArray(name, 256);
		clearArray(lineString, 256);

		strcpy(temp, "./messages/");
		strcat(temp, streamToAdd);
		strcat(temp, "StreamUsers.txt");

		dataFile = fopen(temp, "r");

		/*If the stream exists, check if the user already exists in that stream*/
		while (dataFile != NULL && fgets(lineString, 256, dataFile) != NULL) {

			int i = 0;

			for (i = strlen(lineString)-1; i >= 0; i--) {

				if (lineString[i] == ' ') {

					strncpy(name, lineString, i);
					name[i] = '\0';
					break;
				}
			}

			if (strcmp(name, username) == 0) {

				addUserToStream = false;
				printf("User already exists for the %s stream\n", streamToAdd);
			}
		}

		if (dataFile != NULL) {

			fclose(dataFile);
		}

		/*Add user to stream*/
		if (addUserToStream == true) {

			dataFile = fopen(temp, "a");

			fprintf(dataFile, "%s 0\n", username);

			fclose(dataFile);
		}

		strcpy(temp, "./messages/");
		strcat(temp, streamToAdd);
		strcat(temp, "Stream.txt");
		dataFile = fopen(temp, "a");
		fclose(dataFile);

		strcpy(temp, "./messages/");
		strcat(temp, streamToAdd);
		strcat(temp, "StreamData.txt");
		dataFile = fopen(temp, "a");
		fclose(dataFile);

		streamToAdd = strtok(NULL, " ,");
	}
}

void removeUser(char * username, char * list) {

	FILE * originalFile = NULL;
	FILE * newFile = NULL;

	char * streamToRemove;
	char streamName[256];
	char lineString[256];
	char tempString[256];

	clearArray(lineString, 256);
	clearArray(tempString, 256);

	streamToRemove = strtok(list,",");

	/*Loop through each stream the user entered*/
	while (streamToRemove != NULL) {

		clearArray(streamName, 256);

		strcpy(streamName, "./messages/");
		strcat(streamName, streamToRemove);
		strcat(streamName, "StreamUsers.txt");

		originalFile = fopen(streamName, "r");

		/*Remove the user from the user file by deleting the line that contains their username*/
		if (originalFile != NULL) {

			newFile = fopen("./messages/temp.txt", "w");

			while (fgets(lineString, 256, originalFile) != NULL) {

				int i = 0;

				clearArray(tempString, 256);

				/*Get username from line*/
				for (i = strlen(lineString)-1; i >= 0; i--) {

					if (lineString[i] == ' ') {

						strncpy(tempString, lineString, i);
						tempString[i] = '\0';
						break;
					}
				}

				/*If the username found on the line is not the username to be deleted, than save the line to a temp file*/
				if (strcmp(username, tempString) != 0) {

					fprintf(newFile, "%s", lineString);
				}
			}

			fclose(originalFile);
			fclose(newFile);

			rename("./messages/temp.txt", streamName);
		}

		streamToRemove = strtok(NULL, " ,");
	}
}

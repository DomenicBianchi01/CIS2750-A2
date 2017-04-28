#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parseFile.h"
#include "converter.h"
#include "stream.h"

Node * parseDataFile(FILE * dataFile, Node * list) {

	char lineString[256];
	char string[256];
	int counter = 0;
	int i = 0;
	bool quotation = false;
	bool isComment = false;
	Node * ptr = NULL;

	clearArray(string, 256);

	while (fgets(lineString, 256, dataFile) != NULL) {

		for (i = 0; i < strlen(lineString); i++) {

			if (lineString[i] == '"' && quotation == false) {
				quotation = true;
			}
			else if (lineString[i] == '"' && lineString[i-1] != '\\') {
				quotation = false;
			}

			switch(lineString[i]) {

				case '\n':
				case '\t':
				case '\r':
				case ' ':
				case ',':
				case ';':
				case '(':
				case ')':
				case '{':
				case '}':
				case '.':

                /*If header*/
				if (lineString[i] == '.' && lineString[i+1] == 'h' && (lineString[i+2] == '>' || lineString[i+2] == '"')) {

					string[counter] = lineString[i];
					counter = counter + 1;
					continue;
				}

				if (quotation == true) {

					string[counter] = lineString[i];
					counter = counter + 1;
				}

				if (isComment == true && i == strlen(lineString) - 1) {

					string[counter] = '@';
					counter = counter + 1;
				}

				if (counter != 0 && quotation == false) {

					ptr = createNode(string, "");
					list = addToBack(list, ptr);
					clearArray(string, 256);
					counter = 0;
				}

				if (lineString[i] != ' ' && lineString[i] != '\n' && lineString[i] != '\t' && lineString[i] != '\r' && quotation == false) {

					string[counter] = lineString[i];
					counter = counter + 1;
					ptr = createNode(string, "");
					list = addToBack(list, ptr);
					clearArray(string, 256);
					counter = 0;
				}
				break;
				default:
				string[counter] = lineString[i];
				counter = counter + 1;

                /*Check if line is a comment*/
				if (lineString[i] == '/' && (lineString[i+1] == '/' || lineString[i+1] == '*')) {

					isComment = true;
				}
				break;
			}
		}

		isComment = false;
	}
	fclose(dataFile);
	return list;
}

void saveToFile(Node * list, char fileName[]) {

	FILE * dataFile = NULL;
	int dotIndex = 0;
	int i = 0;
	int bracketCounter = 0;
	bool isComment = false;
	bool endOfComment = false;
	bool indent = true;
	/*bool isLoop = true;*/
	bool forLoop = false;

	char *character = strchr(fileName, '.');
	dotIndex = character - fileName + 1;

    /*Format file name which data will be saved to*/
	for (i = dotIndex; i < strlen(fileName); i++) {

		if (i == dotIndex+1) {

			fileName[i] = '\0';
		}
		else {

			fileName[i] = 'c';
		}
	}

    /*Open the file that all the data will be saved to*/
	dataFile = fopen(fileName, "w");

	while (list != NULL) {

		if (list->key[strlen(list->key)-1] == '^') {
			list = list->next;
			continue;
		}

		if (strcmp(list->key, "{") == 0) {

			bracketCounter = bracketCounter + 1;

			fprintf(dataFile,"{\n");
			indent = true;
		}
		else if (strcmp(list->key, "}") == 0) {

			bracketCounter = bracketCounter - 1;

			if (list->next != NULL && strcmp(list->next->key, ";") == 0) {

				fprintf(dataFile,"}");
			}
			else if (list->next != NULL && strcmp(list->next->key, "while") == 0) {

				if (indent == true) {

					addIndenting(&indent, bracketCounter, dataFile);
				}

				fprintf(dataFile,"}");

				/*isLoop = true;*/
			}
			else {

				if (indent == true) {

					addIndenting(&indent, bracketCounter, dataFile);
				}

				fprintf(dataFile,"}\n");
			}

			indent = true;
		}
		else {

			if (indent == true /*&& isLoop == false*/) {

				addIndenting(&indent, bracketCounter, dataFile);
			}
			/*else if (strcmp(list->key, ";") == 0) {

				isLoop = false;
			}*/

			if ((list->key[0] == '/' && list->key[1] == '/') || (list->key[0] == '/' && list->key[1] == '*')) {

				isComment = true;
			}

			if (strcmp(list->key, "for") == 0) {

				forLoop = true;
			}

			if (strcmp(list->key, ")") == 0) {

				forLoop = false;
			}

			if (list->key[strlen(list->key)-1] == '@') {

				endOfComment = true;
			}

			if (endOfComment == true) {

				list->key[strlen(list->key)-1] = '\0';
			}

            /*If there is a period or semicolon in the text don't put any whitepsace around it*/
            if (list->next != NULL && strcmp(list->key, "userPost") == 0 && strcmp(list->next->key, "*") != 0 && strcmp(list->next->key, ")") != 0) {

            	fprintf(dataFile,"%s * ", list->key);
            	list = list->next;
            	strcpy(list->next->key, "");
            }
			if (list->next != NULL && (strcmp(list->next->key, ".") == 0 || strcmp(list->key, ".") == 0)) {

				fprintf(dataFile,"%s", list->key);
			}
			else {

				fprintf(dataFile,"%s ", list->key);
			}

			if (strlen(list->key) > 0) {

				if (list->key[strlen(list->key)-1] == '>' || (list->key[strlen(list->key)-1] == '"' && list->key[strlen(list->key)-2] == 'h' && list->key[strlen(list->key)-3] == '.') || (list->key[strlen(list->key)-1] == ';' && isComment == false && forLoop == false) || endOfComment == true) {

					fprintf(dataFile,"\n");
					indent = true;
					isComment = false;
					endOfComment = false;
				}
			}
		}

		list = list->next;
	}

	fclose(dataFile);
}

void addIndenting(bool * indent, int counter, FILE * dataFile) {

	int i = 0;

	for (i = counter; i != 0; i--) {

		fprintf(dataFile, "    ");
	}

	*indent = false;
}

void clearArray(char string[], int length) {

	int j = 0;

	/*Set entire array to null terminators*/
	for (j = 0; j < length; j++) {
		
		string[j] = '\0';
	}
}

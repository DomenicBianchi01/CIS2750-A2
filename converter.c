#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "converter.h"
#include "parseFile.h"

Node * createNode(char keyString[], char structName[]) {
	
	Node * current;

	/*Malloc memory for the Tree node*/
	current = malloc(sizeof(Node));

	/*If the malloc fails, do not create the node*/
	if (current == NULL)  {

		free(current);
		return NULL;
	}
	
	strcpy(current->key, keyString);
	strcpy(current->type, "");
	strcpy(current->structMember, structName);
	current->next = NULL;

	return current;
}

ParamNode * createParamNode(char keyString[]) {
	
	ParamNode * current;

	/*Malloc memory for the Tree node*/
	current = malloc(sizeof(ParamNode));

	/*If the malloc fails, do not create the node*/
	if (current == NULL)  {

		free(current);
		return NULL;
	}
	
	strcpy(current->key, keyString);
	current->next = NULL;

	return current;
}

FuncNode * createFuncNode(char nameString[], char idString[]) {
	
	FuncNode * current;

	/*Malloc memory for the Tree node*/
	current = malloc(sizeof(FuncNode));

	/*If the malloc fails, do not create the node*/
	if (current == NULL)  {

		free(current);
		return NULL;
	}
	
	strcpy(current->structName, nameString);
	strcpy(current->variableName, idString);
	current->next = NULL;

	return current;
}

VariableNode * createVariableNode(char nameString[], char typeString[], char methodName[], int scopeFlag) {
	
	VariableNode * current;

	/*Malloc memory for the Tree node*/
	current = malloc(sizeof(VariableNode));

	/*If the malloc fails, do not create the node*/
	if (current == NULL)  {

		free(current);
		return NULL;
	}
	
	strcpy(current->type, typeString);
	strcpy(current->name, nameString);
	strcpy(current->method, methodName);
	current->flag = scopeFlag;
	current->next = NULL;

	return current;
}

Node * addToBack(Node * list, Node * toBeAdded) {

	/*If nothing is to be added*/
	if (toBeAdded == NULL && list != NULL) {
		
		return list;
	}
	else if (toBeAdded == NULL && list == NULL) {

		return NULL;
	}
	/*If list is empty*/
	else if (toBeAdded != NULL && list == NULL) {

		return toBeAdded;
	}

	/*Not on last element*/
	if (list->next != NULL) {

		addToBack(list->next, toBeAdded);
	}
	else {

		list->next = toBeAdded;
	}

	return list;
}

ParamNode * addToBack2(ParamNode * list, ParamNode * toBeAdded) {

	/*If nothing is to be added*/
	if (toBeAdded == NULL && list != NULL) {
		
		return list;
	}
	else if (toBeAdded == NULL && list == NULL) {

		return NULL;
	}
	/*If list is empty*/
	else if (toBeAdded != NULL && list == NULL) {

		return toBeAdded;
	}

	/*Not on last element*/
	if (list->next != NULL) {

		addToBack2(list->next, toBeAdded);
	}
	else {

		list->next = toBeAdded;
	}

	return list;
}

FuncNode * addToBack3(FuncNode * list, FuncNode * toBeAdded) {

	/*If nothing is to be added*/
	if (toBeAdded == NULL && list != NULL) {
		
		return list;
	}
	else if (toBeAdded == NULL && list == NULL) {

		return NULL;
	}
	/*If list is empty*/
	else if (toBeAdded != NULL && list == NULL) {

		return toBeAdded;
	}

	/*Not on last element*/
	if (list->next != NULL) {

		addToBack3(list->next, toBeAdded);
	}
	else {

		list->next = toBeAdded;
	}

	return list;
}

VariableNode * addToBack4(VariableNode * list, VariableNode * toBeAdded) {

	/*If nothing is to be added*/
	if (toBeAdded == NULL && list != NULL) {
		
		return list;
	}
	else if (toBeAdded == NULL && list == NULL) {

		return NULL;
	}
	/*If list is empty*/
	else if (toBeAdded != NULL && list == NULL) {

		return toBeAdded;
	}

	/*Not on last element*/
	if (list->next != NULL) {

		addToBack4(list->next, toBeAdded);
	}
	else {

		list->next = toBeAdded;
	}

	return list;
}

void replaceClassWithStruct(Node * list) {

	Node * listStart = list;
	FuncNode * funcList = NULL;
	VariableNode * variableList = NULL;
	bool isComment = false;
	char tempString[256];

	clearArray(tempString, 256);

	/*variableList = findAllVariableDeclarations(list);*/

	/*Loop through all keywords*/
	while (list != NULL) {

		/*Check for comments*/
		if (strcmp(list->key, "//") == 0 || strcmp(list->key, "/*") == 0) {

			isComment = true;
		}
		else if ((strcmp(&list->key[strlen(list->key)-1], "@") == 0 || strcmp(list->key, "*/") == 0) && isComment == true) {

			isComment = false;
		}

		/*Search for the "class" keyword and replace it with the keyword "struct"*/
		if (strcmp(list->key, "class") == 0 && isComment == false) {

			strcpy(list->key, "struct");

			if (strcmp(list->next->next->key, "{") == 0) {

				modifyWithFunctionPointers(list->next->next, list->next->key);
			}
			/*Add constructor*/
			else {

				Node * ptr = NULL;
				FuncNode * ptr2 = NULL;
				char name[256];
				clearArray(name,256);
				strcpy(name, "constructor");
				strcat(name, list->next->key);
				strcat(name, "(&");
				strcat(name, list->next->next->key);
				strcat(name, ");");

				ptr = createNode(name, "");
				ptr->next = list->next->next->next->next;
				list->next->next->next->next = ptr;

				ptr2 = createFuncNode(list->next->key, list->next->next->key);
				funcList = addToBack3(funcList, ptr2);
			}
		}
		else if (list->next != NULL && strcmp(list->next->key, ".") == 0) {

			FuncNode * dupList = funcList;

			while (dupList != NULL) {

				/*Search for matching struct identifier name (such as: myB)*/
				if (strcmp(dupList->variableName, list->key) == 0) {

					/*modifyFunctionCall(list->next->next, dupList, variableList, listStart);*/
				}

				dupList = dupList->next;
			}
		}

		list = list->next;
	}

	addConstructorMethod(listStart, funcList, variableList);
	updateMain(listStart);

	destroyList2(funcList);
	destroyList3(variableList);
}

void updateMain(Node * list) {

	bool foundMain = false;

	while (list != NULL) {

		if (strcmp(list->key, "main") == 0 && strcmp(list->next->key, "(") == 0) {

			foundMain = true;
			list = list->next;
			continue;
		}

		if (foundMain == true) {

        	if (strcmp(list->key, "readInput") == 0) {

        		strcpy(list->key, "postEntryreadInputci");
        	}
        	else if (strcmp(list->key, "getTimeAndDate") == 0) {

        		strcpy(list->key, "postEntrygetTimeAndDatec");
        	}
        	else if (strcmp(list->key, "formatEntry") == 0) {

        		strcpy(list->key, "postEntryformatEntrycccc");
        	}
        	else if (strcmp(list->key, "submitPost") == 0) {

        		strcpy(list->key, "postEntrysubmitPost");
        	}

		}

		list = list->next;
	}
}

void addConstructorMethod(Node * list, FuncNode * funcList, VariableNode * variableList) {

	bool isComment = false;
	FuncNode * saveFuncList = funcList;
	Node * functionBank = NULL;
	Node * saveBank = functionBank;
	Node * structIDList = NULL;
	Node * saveStructList = NULL;
	Node * ptr = NULL;
	Node * funcPointerBank = NULL;
	char name[1000];
	bool firstTime = true;
	
	clearArray(name,256);

	functionBank = saveBank;

	/*Search for comments*/
	if (list != NULL && (strcmp(list->key, "//") == 0 || strcmp(list->key, "/*") == 0)) {

		isComment = true;
	}
	else if (list != NULL && (strcmp(&list->key[strlen(list->key)-1], "@") == 0 || strcmp(list->key, "*/") == 0) && isComment == true) {

		isComment = false;
	}

	while (list->next != NULL) {

		/*Find the struct body*/
		if (list->next->next != NULL && strcmp(list->key, "struct") == 0 && strcmp(list->next->next->key, "{") == 0 && isComment == false) {

			ptr = createNode(list->next->key, list->next->structMember);
			structIDList = addToBack(structIDList, ptr);
			saveStructList = structIDList;

			if (firstTime == true) {

				firstTime = false;
			}

			/*Find all function pointers within the struct*/
			funcPointerBank = findFunctionPointers(list, funcPointerBank, list->next);
		}
		/*Search for main method*/
		else if (list->next->next != NULL && strcmp(list->next->key, "int") == 0 && strcmp(list->next->next->key, "main") == 0) {

			funcList = saveFuncList;
			functionBank = saveBank;
			while (funcList != NULL) {

				/*Insert constructor method below struct definition*/
				if (strcmp(funcList->structName, structIDList->key) == 0) {

					strcpy(name, "void constructor");
					strcat(name, structIDList->key);
					strcat(name, " (struct ");
					strcat(name, funcList->structName);
					strcat(name, " * ptr");
					strcat(name, ") {\n");

					Node * save = funcPointerBank;

					while (funcPointerBank != NULL) {

						/*Determine if function pointer belongs to the current constructor being created*/
						if (strcmp(funcPointerBank->type, structIDList->key) == 0){

							int starIndex;
							int bracketIndex;

							char temp[256];
							clearArray(temp, 256);

							/*Find substring that contains the function name*/
							char * character = strchr(funcPointerBank->key, '*');
							char * character2 = strchr(funcPointerBank->key, ')');
							starIndex = character - funcPointerBank->key + 1;
							bracketIndex = character2 - funcPointerBank->key;

							strncpy(temp, funcPointerBank->key + starIndex, bracketIndex - starIndex);

							/*Link the struct pointer to the poper function*/
							strcat(name, "    ptr->");
							strcat(name, temp);
							strcat(name, " = ");
							strcat(name, temp);
							strcat(name, ";\n");
						}

						funcPointerBank = funcPointerBank->next;
					}

					strcat(name, "    ptr->postEntryformatEntrycccc = postEntryformatEntrycccc;\n");

					strcat(name, "}\n");

					funcPointerBank = save;

					ptr = createNode(name, "");
					ptr->next = list->next;
					list->next = ptr;

					/*Move onto the next nodes*/
					if (structIDList->next != NULL) {

						structIDList = structIDList->next;
						functionBank = saveBank;
					}
					else {

						list = list->next->next;
						break;
					}
				}

				funcList = funcList->next;
			}
		}

		list = list->next;
	}

	destroyList(funcPointerBank);
	destroyList(saveStructList);
}

Node * findFunctionPointers(Node * list, Node * bank, Node * name) {

	while (list != NULL) {

		/*If found the end of the struct, end the search*/
		if (strcmp(list->key, "}") == 0 && strcmp(list->next->key, ";") == 0) {

			break;
		}
		/*Found a function pointer. Save all pointers so they can be referenced else where in this program*/
		else if (strstr(list->key, "void(*") != NULL || strstr(list->key, "int(*") != NULL || strstr(list->key, "float(*") != NULL || strstr(list->key, "double(*") != NULL || strcmp(list->key, "long(*") == 0 || strcmp(list->key, "short(*") == 0 || strcmp(list->key, "char(*") == 0) {

			Node * ptr = NULL;
			ptr = createNode(list->key, list->structMember);
			strcpy(ptr->type,name->key);
			bank = addToBack(bank, ptr);
		}

		list = list->next;
	}

	return bank;
}

void modifyFunctionCall(Node * list, FuncNode * funcList, VariableNode * variableList, Node * listStart) {

	Node * name = list;
	VariableNode * loop = variableList;
	Node * prevNode = NULL;
	Node * prevPrevNode = NULL;
	VariableNode * prevNode2 = NULL;
	Node * variableName = list->next->next;
	int numberOfParameters = 1;
	int i = 0;
	bool firstComma = true;
	char structName[256];

	clearArray(structName, 256);

	loop = variableList;

	/*Determine how many parameters there are*/
	while (name != NULL && strcmp(name->key, ")") != 0) {

		/*If there are no parameters to the function*/
		if (strcmp(name->key, "(") == 0 && strcmp(name->next->key, ")") == 0) {

			numberOfParameters = 0;
			break;
		}
		/*If the first comma is found, that means there are two parameters*/
		else if (strcmp(name->key, ",") == 0 && firstComma == true) {

			firstComma = false;
			numberOfParameters = numberOfParameters + 2;
		}
		/*Otherwise just add one parameter to the count*/
		else if (strcmp(name->key, ",") == 0) {

			numberOfParameters = numberOfParameters + 1;
		}

		name = name->next;
	}

	name = list;

	while (name != NULL) {

		variableList = loop;

		while (variableList != NULL) {

			/*Check if the method that the variable belongs to is equal to the current keyword*/
			if (strcmp(variableList->method, name->key) == 0 && variableList->flag == 1) {

				Node * ptr = NULL;
				char temp[256];
				char temp2[256];
				char fullName[256];

				clearArray(temp, 256);
				clearArray(temp2, 256);
				clearArray(fullName, 256);

				/*Using the struct declaration, access the variable that is needed. For example, myA.varA*/
				strcpy(temp, prevPrevNode->key);
				strcat(temp, ".");
				strcat(temp, variableList->name);

				if (strcmp(name->next->key, ")") != 0) {

					strcat(temp, ",");
				}

				ptr = createNode(temp, "");
				ptr->next = name->next->next;
				name->next->next = ptr;

				/*Save the first character*/
				strcpy(temp2, variableList->type);
				temp2[1] = '\0';

				FuncNode * saveFuncList = funcList;


				while (funcList != NULL) {

					/*Search for struct declarations*/
					if (strcmp(funcList->variableName, prevPrevNode->key) == 0) {

						strcat(fullName, funcList->structName);
						strcat(fullName, name->key);

						break;
					}

					funcList = funcList->next;
				}

				funcList = saveFuncList;

				strcat(name->key, temp2);
				strcpy(temp, variableList->type);
				strcat(temp, " ");
				strcat(temp, variableList->name);

				addParameterToFunction(listStart, variableList->method, temp, funcList);
				modifyAllFunctionLocations(listStart, fullName, temp2);
				
				if (prevNode2 != NULL) {

					prevNode2->next = variableList->next;
				}

				free(variableList);

				break;
			}

			prevNode2 = variableList;
			variableList = variableList->next;
		}

		prevPrevNode = prevNode;
		prevNode = name;
		name = name->next;
	}

	name = list;
	/*Add struct name to function call (within main method)*/
	strcpy(structName, funcList->structName);
	strcat(structName, name->key);
	strcpy(list->key, structName);

	variableList = findAllVariableDeclarations(listStart);

	loop = variableList;

	/*Add variable letters to function name (i,f,c, etc). Each iteration of the for loop is for each parameter of the function call*/
	for (i = 0; i < numberOfParameters; i++) {

		/*If this the not the first iteration, move onto the next variable to name to find*/
		if (i != 0) {

			variableName = variableName->next->next;
		}

		/*Reset the list back to the start*/
		loop = variableList;

		/*Loop through all variables in the program*/ /*->next*/
		while (loop != NULL) {

			/*If names match, get the variable type and add it on to the function name (for the function call)*/
			if (strcmp(loop->name, variableName->key) == 0) {

				char letter[256];
				strcpy(letter,loop->type);
				letter[1] = '\0';
				strcat(list->key, letter);

				/*Break the loop if we have looped through the entire list*/
				break;
			}

			loop = loop->next;
		}
	}

	destroyList3(variableList);
}

void modifyAllFunctionLocations(Node * list, char functionName[], char stringToAdd[]) {

	char temp[256];
	char nameTemp[256];

	while (list != NULL) {

		/*Look for and update function header (with function body)*/
		if (strcmp(list->key, functionName) == 0) {

			strcat(list->key, stringToAdd);
		}
		/*Look for and update function pointer within struct*/
		else {

			clearArray(temp, 256);
			clearArray(nameTemp, 256);
			int starIndex = 0;
			int bracketIndex = 0;

			/*Find substring that contains the function name*/
			char * character = strchr(list->key, '*');
			char * character2 = strchr(list->key, ')');
			starIndex = character - list->key + 1;
			bracketIndex = character2 - list->key;

			/*Create function pointer*/
			if (character != NULL && character2 != NULL) {

				strncpy(temp, list->key + starIndex, bracketIndex - starIndex);

				if (strcmp(temp, functionName) == 0) {

					strcpy(nameTemp, "void(*");
					strcat(nameTemp, functionName);
					strcat(nameTemp, stringToAdd);
					strcat(nameTemp, ")();");
					strcpy(list->key, nameTemp);
				}
			}
		}

		list = list->next;
	}
}

void addParameterToFunction(Node * list, char methodName[], char variable[], FuncNode * funcList) {

	FuncNode * saveList = funcList;
	char temp[256];

	/*Search for the location of where the new parameter needs to be inserted*/
	while (list != NULL) {

		funcList = saveList;

		while (funcList != NULL) {

			clearArray(temp, 256);

			strcpy(temp, funcList->structName);
			strcat(temp, methodName);

			/*Create a node for the new parameter and add it to the main list*/
			if (strcmp(list->key, temp) == 0) {

				Node * ptr = NULL;

				ptr = createNode(variable, "");
				ptr->next = list->next->next;
				list->next->next = ptr;
				break;
			}

			funcList = funcList->next;
		}

		list = list->next;
	}
}

VariableNode * findAllVariableDeclarations(Node * list) {

	VariableNode * variableList = NULL;
	Node * prevNode = NULL;
	Node * prevPrevNode = NULL;
	int scopeLevel = 0;
	char type[25];
	char methodName[256];
	bool oneLine = false;

	clearArray(methodName, 256);
	clearArray(type, 25);

	while (list != NULL) {

		/*Search for struct or variable declaraton*/
		if ((strcmp(list->key, "struct") == 0 && strcmp(list->next->next->key, "{") == 0) || ((strcmp(list->key, "void") == 0 || strcmp(list->key, "int") == 0 || strcmp(list->key, "float") == 0 || strcmp(list->key, "double") == 0 || strcmp(list->key, "long") == 0 || strcmp(list->key, "short") == 0 || strcmp(list->key, "char") == 0) && strcmp(list->next->next->key, "(") == 0) || (strcmp(list->key,"=") == 0 && strcmp(list->next->next->key, ";") == 0)) {

			/*If variable*/
			if (strcmp(list->key,"=") == 0 && strcmp(list->next->next->key, ";") == 0 && strcmp(prevPrevNode->key, "int") != 0 && strcmp(prevPrevNode->key, "double") != 0 && strcmp(prevPrevNode->key, "float") != 0 && strcmp(prevPrevNode->key, "long") != 0 && strcmp(prevPrevNode->key, "short") != 0 && strcmp(prevPrevNode->key, "char") != 0 && strcmp(methodName, "main") != 0) {

				VariableNode * ptr = NULL;
				ptr = createVariableNode(prevNode->key, type, methodName, 1);
				variableList = addToBack4(variableList, ptr);
			}
			/*If not main method*/
			else if (strcmp(methodName, "main") != 0){

				strcpy(methodName, list->next->key);
			}
		}

		/*Determine scope*/
		if (strcmp(list->key, "{") == 0) {

			scopeLevel = scopeLevel + 1;
		}
		else if (strcmp(list->key, "}") == 0) {

			scopeLevel = scopeLevel - 1;
		}
		/*Multiple declarations on one line*/
		else if (oneLine == true) {

			if (strcmp(list->next->key,";") == 0 || strcmp(list->next->key,")") == 0) {

				oneLine = false;
			}
			else {

				VariableNode * ptr = NULL;
				ptr = createVariableNode(list->next->key, type, methodName, 0);
				variableList = addToBack4(variableList, ptr);
			}
		}
		/*If struct declaration*/
		else if ((strcmp(list->key, "int") == 0 || strcmp(list->key, "float") == 0 || strcmp(list->key, "double") == 0 || strcmp(list->key, "short") == 0 || strcmp(list->key, "long") == 0 || strcmp(list->key, "void") == 0 || strcmp(list->key, "char") == 0) && strcmp(list->next->next->key, "(") != 0) {

			VariableNode * ptr = NULL;
			ptr = createVariableNode(list->next->key,list->key, methodName, 0);
			variableList = addToBack4(variableList, ptr);
			strcpy(type, list->key);

			if (strcmp(list->next->next->key, ",") == 0) {

				oneLine = true;

			}
			else {

				oneLine = false;
			}
		}

		prevPrevNode = prevNode;
		prevNode = list;

		list = list->next;
	}

	return variableList;
}

void modifyWithFunctionPointers(Node * list, char structName[]) {

	char temp[256];
	bool lookForFunctionName = false;
	bool lookForOpenBracket = false;
	bool lookForType = true;
	bool lookForParams = false;
	bool duplicate = false;
	bool inFunctionBody = false;
	Node * functionStart = NULL;
	Node * functionBodyBank = NULL;
	Node * functionBank = NULL;
	ParamNode * paramList = NULL;
	Node * prevNode = NULL;

	clearArray(temp,256);

	/*Loop through all keywords*/
	while (list != NULL) {

		/*Reached the end of the struct*/
		if (strcmp(list->key, "}") == 0 && strcmp(list->next->key, ";") == 0 && functionBank != NULL && prevNode != NULL) {

			insertFunctionPointer(prevNode, functionBank);
			insertFunctionBody(list->next, functionBodyBank);

			break;
		}
		else if (strcmp(list->key, "}") == 0 && strcmp(list->next->key, ";") != 0 && duplicate == false) {
			duplicate = true;

			functionBank = createFunctionPointer(functionStart, functionBank);
			functionBodyBank = saveFunctionBody(functionStart, functionBodyBank);

			inFunctionBody = false;
		}

		/*Search for type*/
		if ((strcmp(list->key, "int") == 0 || strcmp(list->key, "float") == 0 || strcmp(list->key, "double") == 0 || strcmp(list->key, "short") == 0 || strcmp(list->key, "long") == 0 || strcmp(list->key, "void") == 0 || strcmp(list->key, "char") == 0 || strcmp(list->key, "userPost") == 0) && lookForType == true && inFunctionBody == false) {

			lookForFunctionName = true;
			lookForType = false;
			/*Containing type, functionStart->next contains the function name, etc.*/
			functionStart = list;
			prevNode = list;
			list = list->next;

			if (strcmp(list->key, "*") == 0) { /*userpost*/

				list = list->next;
			}


			continue;
		}

		/*Search for function name*/
		if (lookForFunctionName == true) {

			/*If main method, end the search*/
			if (strcmp(list->key, "main") == 0) {

				lookForFunctionName = false;
				lookForType = true;
			}
			else {

				lookForOpenBracket = true;
				lookForFunctionName = false;
			}

			prevNode = list;
			list = list->next;
			continue;
		}

		/*Search for open bracket to determine if function, otherwise end search*/
		if (lookForOpenBracket == true) {

			if (strcmp(list->key, "(") == 0) {

				duplicate = false;

				/*Save struct name to temp*/
				strcpy(temp, structName);
				strcpy(functionStart->next->structMember, structName);
				/*Merge struct name and function name together*/
				if (strcmp(functionStart->next->key, "*") == 0) {

					strcat(structName, functionStart->next->next->key);
				}
				else {

					strcat(structName, functionStart->next->key);
				}
				/*Replace original function name with new function name*/
				strcpy(functionStart->next->key, structName);
				/*Load the struct name back into structName*/
				strcpy(structName, temp);

				lookForOpenBracket = false;
				lookForParams = true;
				prevNode = list;
				list = list->next;
				continue;
			}
			/*Not a function. Restart the proccess*/
			else {

				lookForType = true;
				lookForOpenBracket = false;
				prevNode = list;
				list = list->next;
				continue;
			}
		}

		/*Search for parameters*/
		if (lookForParams == true) {

			ParamNode * ptr = NULL;

			if (strcmp(list->key, ")") == 0) {

				lookForParams = false;
				lookForType = true;
				inFunctionBody = true;
				bool firstTime = true;

				/*Merge first leter of each node into the function name*/
				while (paramList !=  NULL) {

					paramList->key[1] = '\0';
					strcat(functionStart->next->key, paramList->key);

					/*int*/
					if (strcmp(paramList->key, "i") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "int");
					}
					/*float*/
					else if (strcmp(paramList->key, "f") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "float");
					}
					/*double*/
					else if (strcmp(paramList->key, "d") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "double");
					}
					/*char*/
					else if (strcmp(paramList->key, "c") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "char*");
					}
					/*short*/
					else if (strcmp(paramList->key, "s") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "short");
					}
					/*long*/
					else if (strcmp(paramList->key, "l") == 0) {

						if (firstTime == false) {

							strcat(functionStart->next->type, ",");
						}

						firstTime = false;

						strcat(functionStart->next->type, "long");
					}

					ptr = paramList->next;
					free(paramList);
					paramList = ptr;
				}
			}

			/*Save the parameter type*/
			if (strcmp(list->key, "int") == 0 || strcmp(list->key, "float") == 0 || strcmp(list->key, "double") == 0 || strcmp(list->key, "short") == 0 || strcmp(list->key, "long") == 0 || strcmp(list->key, "char") == 0|| strcmp(list->key, "void") == 0) {

				ptr = createParamNode(list->key);
				paramList = addToBack2(paramList, ptr);
			}
		}

		prevNode = list;
		list = list->next;
	}
}

Node * createFunctionPointer(Node * functionInfo, Node * functionBank) {

	Node * ptr = NULL;
	char temp[256];
	clearArray(temp,256);

	/*Create the function pointer string; functionInfo is the type, functionInfo->next is the name*/
	strcat(temp, functionInfo->key);

	if (strcmp(functionInfo->key, "userPost") == 0) {

		strcat(temp, "*");
	}

	strcat(temp, "(*");
	strcat(temp, functionInfo->next->key);
	strcat(temp, ")(");
	strcat(temp, functionInfo->next->type);
	strcat(temp, ");");

	ptr = createNode(temp, "");
	functionBank = addToBack(functionBank, ptr);

	return functionBank;
}

Node * saveFunctionBody(Node * list, Node * functionBank) {

	Node * ptr = NULL;
	int count = 0;
	Node * tempList = list;
	bool foundBracket = false;
	bool firstTime = true;

	while ((count != 0 || foundBracket == false)) {

		/*If found "{"" that means everything after up until the closing } is a function body. "count" makes sure the proper "}" marks the end of the function*/
		if (strcmp(tempList->key, "{") == 0) {

			foundBracket = true;
			count = count + 1;
		}
		else if (strcmp(tempList->key,"}") == 0) {

			count = count - 1;
		}

		ptr = createNode(tempList->key, tempList->structMember);
		functionBank = addToBack(functionBank, ptr);

		if (firstTime == true) {

			firstTime = false;
		}

		/*Add an identifier that this keyword is part of a function body*/
		strcat(list->key, "^");

		tempList = tempList->next;
		list = list->next;
	}

	return functionBank;
}

void insertFunctionPointer(Node * list, Node * functionBank) {

	/*Every node in the function bank contains a entire prototype. Insert all the prototypes*/
	while (functionBank != NULL) {

		Node * temp = functionBank->next;
		functionBank->next = list->next;
		list->next = functionBank;
		functionBank = temp;
	}
}

void insertFunctionBody(Node * list, Node * functionBank) {

	Node * save = functionBank;

	/*Find the last node*/
	while (save->next != NULL) {

		save = save->next;
	}

	/*Insert the function body*/
	Node * temp = list->next;
	list->next = functionBank;
	save->next = temp;
}

void destroyList(Node * list) {

	if (list == NULL) {

		return;
	}

	if (list->next != NULL) {

		destroyList(list->next);
	}

	/*Free node from memory*/
	free(list);
}

void destroyList2(FuncNode * list) {

	if (list == NULL) {

		return;
	}

	if (list->next != NULL) {

		destroyList2(list->next);
	}

	/*Free node from memory*/
	free(list);
}

void destroyList3(VariableNode * list) {

	if (list == NULL) {

		return;
	}

	if (list->next != NULL) {

		destroyList3(list->next);
	}

	/*Free node from memory*/
	free(list);
}

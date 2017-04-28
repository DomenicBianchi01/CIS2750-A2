#ifndef __DBIANCHI_CONVERTER__
#define __DBIANCHI_CONVERTER__

/*This struct is designed to hold a standard keyword*/
struct Node {

	char key[1000];
	char type[256];
	char structMember[256];
	struct Node * next;
};

/*This struct is designed to hold ketwords that represent paramaters to a function*/
struct ParamNode {

	char key[256];
	struct ParamNode * next;
};

/*This struct keeps track of all struct declarations (such as struct A myA)*/
struct FuncNode {

	char structName[256];
	char variableName[256];
	struct FuncNode * next;
};

/*This sruct keeps track of all variable declarations*/
struct VariableNode {

	char type[10];
	char name[256];
	char method[256];
	int flag;
	struct VariableNode * next;
};

typedef struct Node Node;
typedef struct ParamNode ParamNode;
typedef struct FuncNode FuncNode;
typedef struct VariableNode VariableNode;

/**
 * createNode
 * Creates new node using malloc
 * IN: char keyString[]
 * OUT: Node * current
 * POST: New node for a keyword
 * ERROR: If malloc fails, node will not be created
 **/
Node * createNode(char keyString[], char structName[]);

/**
 * createParamNode
 * Creates new node using malloc
 * IN: char keyString[]
 * OUT: ParamNode * current
 * POST: New node for a keyword that represents a parameter
 * ERROR: If malloc fails, node will not be created
 **/
ParamNode * createParamNode(char keyString[]);

/**
 * createFuncNode
 * Creates new node using malloc
 * IN: char keyString[], char idString[]
 * OUT: FuncNode * current
 * POST: New node for a keyword(s) that represents a struct declaration
 * ERROR: If malloc fails, node will not be created
 **/
FuncNode * createFuncNode(char keyString[], char idString[]);

/**
 * createVariableNodee
 * Creates new node using malloc
 * IN: char nameString[], char typeString[], char methodName[], int scopeFlag
 * OUT: VariableNode * current
 * POST: New node for a keyword that represents a variable
 * ERROR: If malloc fails, node will not be created
 **/
VariableNode * createVariableNode(char nameString[], char typeString[], char methodName[], int scopeFlag);

/**
 * addToBack
 * Adds a node of type Node to the back of the specified linked list
 * IN: Node * list, Node * toBeAdded
 * OUT: Node * list
 * POST: Node added to the back of the specifed linked list. If this is the first element to be added to the linked list, the first node is returned.
 * ERROR: If either parameter is NULL, nothing will be added to the list
 **/
Node * addToBack(Node * list, Node * toBeAdded);

/**
 * addToBack2
 * Adds a node of type ParamNode to the back of the specified linked list
 * IN: ParamNode * list, ParamNode * toBeAdded
 * OUT: ParamNode * list
 * POST: Node added to the back of the specifed linked list. If this is the first element to be added to the linked list, the first node is returned.
 * ERROR: If either parameter is NULL, nothing will be added to the list
 **/
ParamNode * addToBack2(ParamNode * list, ParamNode * toBeAdded);

/**
 * addToBack3
 * Adds a node of type FuncNode to the back of the specified linked list
 * IN: FuncNode * list, FuncNode * toBeAdded
 * OUT: FuncNode * list
 * POST: Node added to the back of the specifed linked list. If this is the first element to be added to the linked list, the first node is returned.
 * ERROR: If either parameter is NULL, nothing will be added to the list
 **/
FuncNode * addToBack3(FuncNode * list, FuncNode * toBeAdded);

/**
 * addToBack4
 * Adds a node of type VariableNode to the back of the specified linked list
 * IN: VariableNode * list, VariableNode * toBeAdded
 * OUT: VariableNode * list
 * POST: Node added to the back of the specifed linked list. If this is the first element to be added to the linked list, the first node is returned.
 * ERROR: If either parameter is NULL, nothing will be added to the list
 **/
VariableNode * addToBack4(VariableNode * list, VariableNode * toBeAdded);

/**
 * findAllVariableDeclarations
 * Search through all keywords from the C++ code and save every variable declaration (type, value, name)
 * IN: Node * list
 * OUT: Node * variableList
 * POST: A list containing all the variables is returned (one node per variable)
 * ERROR: If list is NULL an error may occur
 **/
VariableNode * findAllVariableDeclarations(Node * list);

/**
 * addConstructorMethod
 * Search for a struct declaration and create a constructor for the struct type. Also, insert the constructor function call for each instance of the struct
 * IN: Node * list, FuncNode * funcList, VariableNode * variableList
 * OUT: NONE
 * POST: A constructor for the struct type is created. Also inserts a function call to the constructor for each instance of the struct.
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
void addConstructorMethod(Node * list, FuncNode * funcList, VariableNode * variableList);

/**
 * replaceClassWithStruct
 * Replace all classes with structs
 * IN: Node * list
 * OUT: NONE
 * POST: A struct replaces the class
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
void replaceClassWithStruct(Node * list);

/**
 * modifyFunctionCall
 * Modify function calls by adding variable types to the function name and adding any additonal parameters that are required
 * IN: Node * list, FuncNode * funcList, VariableNode * variableList, Node * listStart
 * OUT: NONE
 * POST: Function calls are updated
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
void modifyFunctionCall(Node * list, FuncNode * funcList, VariableNode * variableList, Node * listStart);

/**
 * modifyWithFunctionPointers
 * Add function pointers into the code
 * IN: Node * list, char structName[]
 * OUT: NONE
 * POST: Function pointers are created and added
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
void modifyWithFunctionPointers(Node * list, char structName[]);

/**
 * createFunctionPointer
 * Format and malloc the function pointer and add it to the bank of pointers
 * IN: Node * functionInfo, Node * functionBank
 * OUT: Node * functionBank
 * POST: Function pointer is created and added to the bank of pointers to be added into the code later
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
Node * createFunctionPointer(Node * functionInfo, Node * functionBank);

/**
 * saveFunctionBody
 * Save the body of functions within a former class
 * IN: Node * list, Node * functionBank
 * OUT: Node * functionBank
 * POST: Function body is saved and added to the bank/list containing all the function bodies which is to be added into the code later
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
Node * saveFunctionBody(Node * list, Node * functionBank);

/**
 * findFunctionPointers
 * Find all function pointers and save them
 * IN: Node * list, Node * bank, Node * name
 * OUT: Node * functionBank
 * POST: Returns bank/list of all functionPointers
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
Node * findFunctionPointers(Node * list, Node * bank, Node * name);

/**
 * insertFunctionPointer
 * Insert function pointers into the main list/new code
 * IN: Node * list, Node * functionBank
 * OUT: NONE
 * POST: All function pointers are added to the code
 * ERROR: If not all of a node was saved correctly, the function pointer inserted into the code may not be correct
 **/
void insertFunctionPointer(Node * list, Node * functionBank);

/**
 * insertFunctionBody
 * Insert function bodies into the main list/new code
 * IN: Node * list, Node * functionBank
 * OUT: NONE
 * POST: All function bdoies are added to the code
 * ERROR: If not all of a node was saved correctly, the function body inserted into the code may not be correct
 **/
void insertFunctionBody(Node * list, Node * functionBank);

/**
 * modifyAllFunctionLocations
 * Sometimes some or all function pointers may need to be modified twice. This function takes care of that
 * IN: Node * list, char functionName[], char stringToAdd[]
 * OUT: NONE
 * POST: Any function pointers that needed to be modifed again are modified
 * ERROR: If a function pointer is changed when it is not supposed to be, the C code will likely not compile
 **/
void modifyAllFunctionLocations(Node * list, char functionName[], char stringToAdd[]);

void updateMain(Node * list);

/**
 * addParameterToFunction
 * Adds parameter(s) to the function
 * IN: Node * list, char methodName[], char variable[], FuncNode * funcList
 * OUT: NONE
 * POST: Parameters are added to the function
 * ERROR: If malloc fails, node will not be created, and errors may occur
 **/
void addParameterToFunction(Node * list, char methodName[], char variable[], FuncNode * funcList);

/**
 * destroyList
 * Frees all nodes in the list from memory. This function is used to release lists of type Node.
 * IN: Node * list
 * OUT: NONE
 * POST: ALl nodes released from memory
 * ERROR: If the function tries to free a NULL node or invalid pointer, an error will occur
 **/
void destroyList(Node * list);

/**
 * destroyList2
 * Frees all nodes in the list from memory. This function is used to release lists of type FuncNode.
 * IN: FuncNode * list
 * OUT: NONE
 * POST: ALl nodes released from memory
 * ERROR: If the function tries to free a NULL node or invalid pointer, an error will occur
 **/
void destroyList2(FuncNode * list);

/**
 * destroyList3
 * Frees all nodes in the list from memory. This function is used to release lists of type VariableNode.
 * IN: VariableNode * list
 * OUT: NONE
 * POST: ALl nodes released from memory
 * ERROR: If the function tries to free a NULL node or invalid pointer, an error will occur
 **/
void destroyList3(VariableNode * list);

#endif

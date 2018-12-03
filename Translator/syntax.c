#include "translator.h"
#include "lexical.h"
#include "syntax.h"

int decStack[STACK_SIZE];
int decStackPointer = 0;

char *typeStack[STACK_SIZE];
int typeStackPointer = 0;

function functions[MAX_AMOUNT_OF_FUNCTIONS];
int amountOfFunctions = 0;
int currentFunction = 0;

extern getBackPosition;
extern FILE *input;
extern lex cur_lex;

int syntax_manager() {
	if ((input = fopen("D:\\lex_analysis.txt", "r")) == NULL)
		return NOT_FOUND;

	prog();
}

int prog() {
	getLex();
	if (eq("PROGRAM"))
		name();
	else
		// PROGRAM is missed
		error("PROGRAM is missed");

	getLex();
	if (eq("VAR"))
		decList();
	else
		// VAR is missed
		error("VAR is missed");

	// Declarations of functions
	getLex();
	if (eq("FUNCTION")) {
		functionList();
	}
	
	currentFunction = 0;

	// Main function
	getLex();
	if (eq("BEGIN")) {
		stmtList();
	}else {
		error("BEGIN or function declaration is expected");
	}

	if (!eq("END."))
		// END is missed
		error("END is missed");
}

int name() {
	getLex();
	if (!isId())
		error("Prohibited name");
	function *temp = &functions[currentFunction];
	strcpy(temp->name, TID.table + cur_lex.numberInTable * TID.word_size);
}

int decList() {
	while (1) {
		dec();

		getLex();
		if (!eq(";"))
			// Missed ';'
			error("Missed ';'");

		getLex();
		if (!isId()) {
			// There's no other declarations
			getBack();
			break;
		}else {
			getBack();
		}
	}
}

int dec() {
	idList();
	getLex();
	if (eq(":")) {
		getLex();
		if (eq("INTEGER") || eq("REAL")) {
			defineType();
		}else {
			// Missed type
			error("Missed type");
		}
	}else {
		// Missed ':'
		error("Missed ':'");
	}
}

// Give to all variables within the stack a type
int defineType() {
	function *temp;
	int index;
	while (decStackPointer != 0) {
		// Pop out of stack
		int numberInTable = ipop();
		// Multiple declaration
		if (isDeclared(index = map(numberInTable)))
			error("Multiple declaration");
		// Declaration
		temp = &functions[currentFunction];
 		temp->varDeclarations[index].isDeclared = 1;
		strcpy(temp->varDeclarations[index].type, find());
		temp->sizeRec++;
	}
}

// Parse string of variables
int idList() {
	function *temp;
	decStackPointer = 0;
	while (1) {
		getLex();
		if (isId()) {
			// Push into stack
			ipush(cur_lex.numberInTable);
			// Add variables to given function
			if (!isDeclared(map(cur_lex.numberInTable))) {
				temp = &functions[currentFunction];
				temp->variables[temp->sizeVar++] = find();
			}
			getLex();
			if (!eq(",")) {
				// Parsing is probably done
				getBack();
				break;
			}
		}else {
			// Incomplete declaration list
			error("Incomplete declaration list");
		}
	}
}

// Stmt control ';'
int stmtList() {
	while (1) {
		stmt();
		 
		getLex();
		if (!eq(";"))
			// Expected ';'
			error("Expected ';'");
		getLex();
		if (eq("END.") || eq("END") || eq("RETURN")) {
			getBack();
			break;
		}else {
			getBack();
		}
	}
}

int stmt() {
	getLex();

	if (eq("READ")) {
		read();
	}else if (eq("WRITE")) {
		write();
	}else if (eq("FOR")) {
		for_loop();
	}else if (isId()) {
		// assign
		assign();
	}else if (eq("FUNCTION")) {
		// Function call

	}else {
		// Undefined statement 
		error("Undefined statement");
	}

}

int assign() {
	function *temp = &functions[currentFunction];
	typeStackPointer = 0;
	tpush(temp->varDeclarations[map(cur_lex.numberInTable)].type);
	if (!isDeclared(map(cur_lex.numberInTable)))
		// Undeclared variable
		error("Undeclared variable");

	getLex();
	if (!eq(":="))
		// Missed ':='
		error("Missed ':='");

	tpush(":=");
	expression();

	// Checks compatible of operands
	checkOp();
}

int expression() {
	char *type;
	int bracketCount = 0;
	int isSignNow = 0;
	function *temp;

	while (1) {
		getLex();

 		if (isId() && !isSignNow) {
			if (!isDeclared(map(cur_lex.numberInTable)))
				// Undeclared variable
				error("Undeclared variable");

			temp = &functions[currentFunction];
			type = temp->varDeclarations[map(cur_lex.numberInTable)].type;
			tpush(type);
			isSignNow = 1;
		}else if (isNum() && !isSignNow) {
			// Push type of constant into stack
			type = typeOfConstant();
			tpush(type);
			isSignNow = 1;
		}else if (eq("(")) {
			bracketCount++;
		}else if (eq(")") && isSignNow) {
			bracketCount--;
		}else if (isSignNow) {
			if (!operationSign()) {
				if (bracketCount > 0)
					error("Expected ')'");
				else if (bracketCount < 0)
					error("Expected '('");

				getBack();
				return 1;
			}
			isSignNow = 0;
		}else {
			// Operand is missed
			error("Operand is missed");
		}
	}
}

int operationSign() {
	// Undefined expression
	if (!(eq("*") || eq("-") || eq("+") || eq("DIV")))
		return 0;

	tpush("*");
	return 1;
}

//	Unallowed only assign operands of different types
char* isCompatible(char *op, char *type_1, char *type_2) {
	if (!strcmp(op, ":=")) {
		if (!strcmp(type_1, type_2))
			return type_1;
		else
			return "no";
	}

	if (!strcmp(type_1, "REAL") || !strcmp(type_2, "REAL"))
		return "REAL";
	else
		return "INTEGER";
	
	return 1;
}

void checkOp() {
	char *op, *type_1, *type_2, *res;

	while (typeStackPointer != 1) {
		type_2 = tpop();
		op = tpop();
		type_1 = tpop();

		res = isCompatible(op, type_1, type_2);

		if (strcmp(res, "no"))
			tpush(res);
		else
			// Assignment operands of different types
			error("Assignment operands of different types");
	}
}

// return 1, if declared
int isDeclared(int i) {
	if (i == NOT_FOUND) return 0;
	function *temp = &functions[currentFunction];
	return temp->varDeclarations[i].isDeclared == 1 ? 1 : 0;
}

int map(int numberInTable) {
	function *func = &functions[currentFunction];
	for (int i = 0; i < func->sizeVar; i++) {
		if (!strcmp(func->variables[i], TID.table + numberInTable * TID.word_size))
			return i;
	}

	return NOT_FOUND;
}

int read() {
	function *temp = &functions[currentFunction];
	int numberInTable;
	getLex();
	if (eq("(")) {
		idList();
		// Checks whether variables are declared
		while (decStackPointer != 0) {
			numberInTable = ipop();
			if (temp->varDeclarations[map(numberInTable)].isDeclared != 1)
				// Undeclared variable
				error("Undeclared variable");
		}

		getLex();
		if (!eq(")"))
			// Missed ')'
			error("Missed ')'");
	}else {
		// Missed '('
		error("Missed '('");
	}
}

int write() {
	function *temp = &functions[currentFunction];
	int numberInTable;
	getLex();
	if (eq("(")) {
		idList();
		// Checks whether variables are declared
		while (decStackPointer != 0) {
			numberInTable = ipop();
			if (temp->varDeclarations[map(numberInTable)].isDeclared != 1)
				// Undeclared variable
				error("Undeclared variable");
		}

		getLex();
		if (!eq(")"))
			// Missed ')'
			error("Missed ')'");
	}
	else {
		// Missed '('
		error("Missed '('");
	}
}

int for_loop() {
	getLex();
	assign();

	getLex();
	if (!eq("TO"))
		// Missed TO
		error("Missed TO");

	expression();

	getLex();

	if (!eq("DO"))
		// Missed DO
		error("Missed DO");

	body();
}

int body() {
	getLex();

	if (eq("BEGIN")) {
		stmtList();
		getLex();
		if (!eq("END"))
			// Missed END
			error("Missed END");;
	}else {
		getBack();
		stmt();
	}

}

int functionList() {
	while (1) {
		func();

		getLex();
		if (!eq("FUNCTION") && !eq("BEGIN")) {
			error("BEGIN or function declaration is expected");
		}else if (eq("BEGIN")) {
			getBack();
			return 1;
		}
	}
}

int func() {
	currentFunction = ++amountOfFunctions;
	// Function name
	name();

	getLex();
	if (!eq("("))
		error("Missed '('");

	// Arguments of function
	argList();

	getLex();
	if (!eq(")"))
		error("Missed ')'");

	getLex();
	if (!eq(":"))
		error("Missed ':' in function type");

	getLex();
	if (!eq("REAL") && !eq("INTEGER"))
		error("Function type is undefined");
	
	getLex();
	if (!eq("BEGIN"))
		error("BEGIN is expected");

	stmtList();

	getLex();
	if (!eq("RETURN"))
		error("function must return the value");

	// Return expression
//	tpush(functions[currentFunction]);
	expression();

	getLex();
	if (!eq(";"))
		error("Missed ';'");

	getLex();
	if (!eq("END"))
		error("END is missed");
}

int argList() {
	while (1) {
		dec();

		getLex();
		if (!eq(",") && !eq(")"))
			error("Missed ','");

		if (eq(")")) {
			getBack();
			break;
		}
	}
}

// Type of the constant
char *typeOfConstant() {
	for (int i = 0; i < MAX_NUM_SIZE; i++) {
		// TNUM.table[cur_lex.numberInTable][i]
		if(*(TNUM.table + cur_lex.numberInTable * TNUM.word_size + i) == '.')
			return "REAL";
	}

	return "INTEGER";
}

// return 0 - lexeme table != TID
int isId() {
	return cur_lex.table == TID.table_number;
}

// return 0 - lexeme talbe != TNUM
int isNum() {
	return cur_lex.table == TNUM.table_number;
}

int eq(char *s) {
	return !strcmp(find(), s);
}

char* find() {
	switch (cur_lex.table) {
	case 1:
		return TW.table + cur_lex.numberInTable * TW.word_size;
		break;
	case 2:
		return TD.table + cur_lex.numberInTable * TD.word_size;
		break;
	case 3:
		return TNUM.table + cur_lex.numberInTable * TNUM.word_size;
		break;
	case 4:
		return TID.table + cur_lex.numberInTable * TID.word_size;
		break;
	}
}

void ipush(int i) {
	decStack[decStackPointer] = i;
	decStackPointer++;
}

int ipop() {
	decStackPointer--;
	return decStack[decStackPointer];
}

void tpush(char *type) {
	typeStack[typeStackPointer] = type;
	typeStackPointer++;
}

char* tpop() {
	typeStackPointer--;
	char * type = typeStack[typeStackPointer];
	typeStack[typeStackPointer] = NULL;
	return type;
}

int getBack() {
	fseek(input, getBackPosition, SEEK_SET);
}
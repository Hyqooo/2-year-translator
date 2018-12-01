#include "translator.h"
#include "lexical.h"
#include "syntax.h"

int decStack[STACK_SIZE];
int decStackPointer = 0;

char *typeStack[STACK_SIZE];
int typeStackPointer = 0;

extern getBackPosition;
extern FILE *input;
extern lex cur_lex;

int syntax_manager() {
	if ((input = fopen("D:\\lex_analysis.txt", "r")) == NULL)
		return NOT_FOUND;

	prog();

	prVar();
}

int prVar() {
	for (int i = 0; i < TID.size; i++) {
		printf("%d-%s\n", TID.table_r[i].isDeclared, TID.table_r[i].type);
	}
}

int prog() {
	getLex();
	if (eq("PROGRAM"))
		progName();
	else
		// PROGRAM is missed
		error("PROGRAM is missed");

	getLex();
	if (eq("VAR"))
		decList();
	else
		// VAR is missed
		error("VAR is missed");

	// == here ==
	getLex();
	if (eq("BEGIN"))
		stmtList();
	else if (eq("FUNCTION"))
		functionList();
	else
		error("BEGIN or function declaration is expected");

	if (!eq("END."))
		// END is missed
		error("END is missed");
}

int progName() {
	getLex();
	if (!isId())
		error("Prohibited name of the program");
	TID.table_r[cur_lex.numberInTable].isDeclared = 1;
	strcpy(TID.table_r[cur_lex.numberInTable].type, "name");
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

// Give to all variables within the stack the type
int defineType() {
	while (decStackPointer != 0) {
		// Pop out of stack
		int numberInTable = ipop();
		// Multiple declaration
		if (isDeclared(numberInTable))
			error("Multiple declaration");
		// Declaration
		TID.table_r[numberInTable].isDeclared = 1;
		strcpy(TID.table_r[numberInTable].type, find());
	}
}

// Parse string of variable
int idList() {
	decStackPointer = 0;
	while (1) {
		getLex();
		if (isId()) {
			// Push into stack
			ipush(cur_lex.numberInTable);
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
		if (eq("END.")) {
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
	}else {
		// Undefined statement 
		error("Undefined statement");
	}

}

int assign() {
	lex leftSideVar = cur_lex;
	if (!isDeclared(cur_lex.numberInTable))
		// Undeclared variable
		error("Undeclared variable");

	getLex();
	if (!eq(":="))
		// Missed ':='
		error("Missed ':='");

	expression();


}

int expression() {
	char *type;
	int bracketCount = 0;
	int isSignNow = 0;

	typeStackPointer = 0;

	while (1) {
		getLex();

 		if (isId() && !isSignNow) {
			if (!isDeclared(cur_lex.numberInTable))
				// Undeclared variable
				error("Undeclared variable");

			type = TID.table_r[cur_lex.numberInTable].type;
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

	if (strcmp(type_1, "REAL") || strcmp(type_2, "REAL"))
		return "REAL";
	else
		return "INTEGER";
	
	return 1;
}

void checkOp() {
	char *op, *type_1, *type_2, *res;

	type_2 = tpop();
	op = tpop();
	type_1 = tpop();

	res = isCompatible(op, type_1, type_2);

	if (strcmp(res, "no"))
		tpush(res);
	else
		// Assignment operands of different types
		error(1);
}

// return 1, if declared
int isDeclared(int i) {
	return TID.table_r[i].isDeclared == 1 ? 1 : 0;
}

int read() {
	int numberInTable;
	getLex();
	if (eq("(")) {
		idList();
		// Checks whether variables are declared
		while (decStackPointer != 0) {
			numberInTable = ipop();
			if (TID.table_r[numberInTable].isDeclared != 1)
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
	int numberInTable;
	getLex();
	if (eq("(")) {
		idList();
		// Checks whether variables are declared
		while (decStackPointer != 0) {
			numberInTable = ipop();
			if (TID.table_r[numberInTable].isDeclared != 1)
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

}

int functionList() {

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
	return typeStack[typeStackPointer];
}

int getBack() {
	fseek(input, getBackPosition, SEEK_SET);
}
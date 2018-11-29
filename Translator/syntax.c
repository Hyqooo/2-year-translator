#include "translator.h"
#include "lexical.h"
#include "syntax.h"

int decStack[STACK_SIZE];
int stackPointer = 0;

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
		error(1);

	getLex();
	if (eq("VAR"))
		decList();
	else
		// VAR is missed
		error(1);

	// == here ==
	if (eq("BEGIN"))
		stmtList();
	else if (eq("FUNCTION"))
		functionList();
	else
		error(1);

	getLex();
	if (!eq("END."))
		// END is missed
		error(1);
}

int progName() {
	getLex();
	if (!isId())
		error(1);
	TID.table_r[cur_lex.numberInTable].isDeclared = 1;
}

int decList() {
	while (1) {
		dec();

		if (!eq(";")) {
			if (eq("BEGIN"))
				break;
			else
				// Expected ';'
				error(1);
		}
			
	}
}

int dec() {
	while (1) {
		getLex();
		if (isId()) {
			// Push into stack
			ipush(cur_lex.numberInTable);
			getLex();
			if (!eq(",") && !eq(":")) {
				// Expected ','
				error(1);
			}else if (stackPointer && eq(":")) {
				getLex();
				if (eq("INTEGER") || eq("REAL")) {
					while (stackPointer != 0) {
						// Pop out of stack
						int numberInTable = ipop();
						if (TID.table_r[numberInTable].isDeclared == 1)
							// Multiple declaration
							error(1);
						TID.table_r[numberInTable].isDeclared = 1;
						strcpy(TID.table_r[numberInTable].type, find());
					}
				}
				else {
					// Missed type
					error(1);
				}

				stackPointer = 0;
				getLex();
				break;
			}
		}else {
			return 1;
		}
	}
}

int stmtList() {

}

int functionList() {

}

// return 0 - lexeme table != TID
int isId() {
	return cur_lex.table == TID.table_number;
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
	decStack[stackPointer] = i;
	stackPointer++;
}

int ipop() {
	stackPointer--;
	return decStack[stackPointer];
}
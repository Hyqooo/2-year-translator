#ifndef LEXICAL
#define LEXICAL

/*
	 table - table number
	 numberInTable - lexeme number in the table
*/
typedef struct lex {
	int table;
	int numberInTable;
}lex;

/*
	isDeclared - Is variable delcared already?
	type - type of the variable
*/
typedef struct record {
	int isDeclared;
	char *type;
}record;

/*
	table - points to needed table
	size - size of the table
	word_size - maximum size of one word in the table
	table_number - number of table from 1 to 4
*/
typedef struct tabl {
	char *table;
	record *table_r;
	int size;
	int word_size;
	int table_number;
}tabl;

void clear();
void add();
void makelex();
void gc();
void lex_analyzer();
void makeInternalRep(int, int);
void delimiterParser();
void printTable(tabl);
int getLexNumber();
int lexManager();
int isConstant();
int isLegalId();
int look(tabl);
int putl(tabl*);
void tabl_init();

/*
	Globals
*/
tabl TW;
tabl TD;
tabl TID;
tabl TNUM;

#endif // !LEXICAL

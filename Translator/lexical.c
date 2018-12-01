#include "lexical.h"
#include "translator.h"

// Key words table
char TW_char[][WORD_SIZE_IN_TABLE_TW] = {
	"PROGRAM",
	"VAR",
	"BEGIN",
	"END",
	"INTEGER",
	"REAL",
	"READ",
	"WRITE",
	"FOR",
	"DO",
	"TO",
	"FUNCTION",
	"RETURN",
	"IF",
	"THEN",
	"ELSE IF",
	"OR",
	"AND",
	"DIV",
	"END."
};

// Delimiters table
char TD_char[][WORD_SIZE_IN_TABLE_TD] = {
	",",
	";",
	"+",
	"-",
	"*",
	":=",
	"==",
	"!=",
	">",
	"<",
	":",
	"(",
	")"
};

// Identifiers table and numeric constants table respectively
char **TID_char, **TNUM_char;

extern char ch;
extern char buf[BUFFER_SIZE];
extern int buff_index;

extern FILE *input;
extern FILE *output;

lex cur_lex;

long getBackPosition;

int lexManager() {
	tabl_init();

	if ((input = fopen("D:\\source.txt", "r")) == NULL)
		return NOT_FOUND;
	
	if ((output = fopen("D:\\lex_analysis.txt", "w")) == NULL)
		return NOT_FOUND;

	lex_analyzer();

	printf("\nIdentifiers:\n");
	printTable(TID);
	printf("Constants:\n");
	printTable(TNUM);

	fclose(input);
	fclose(output);
	input = NULL;

	return 0;
}

void printTable(tabl t) {
	for (int i = 0; i < t.size; i++) {
		printf("%d)%s\n", i + 1, t.table + i * t.word_size);
	}
}

void lex_analyzer() {
	while (1) {
		gc();
		if (!isalnum(ch)) {
			if (ch == '.') {
				add();
				continue;
			}
			makelex();
			delimiterParser();
			if (ch == EOF) break;
			continue;
		}
		add();
	}
}

void getLex() {
	getBackPosition = ftell(input);
	cur_lex.table = getLexNumber();
	cur_lex.numberInTable = getLexNumber();
}

int getLexNumber() {
	clear();
	while (1) {
		gc();
		if (isdigit(ch))
			add();
		else if (ch == ',' || ch == '>')
			return parseInt();
		else if (ch == EOF)
			return 0;
	}
}

void delimiterParser() {
	int indexInTable;
	char firstDel[2];

	if (ch == EOF) return;

	firstDel[0] = ch;
	firstDel[1] = '\0';

	clear();

	if (isspace(ch)) {
		fprintf(output, "%c", ch);
		return;
	}

	add();
	gc();
	add();
	if ((indexInTable = look(TD)) == NOT_FOUND) {
		clear();
		strcpy(buf, firstDel);
		if ((indexInTable = look(TD)) == NOT_FOUND)
			error(0);
		fseek(input, -1, SEEK_CUR);
	}

	makeInternalRep(TD.table_number, indexInTable);

	clear();
}

void clear() {
	for (int i = 0; i < buff_index; i++) {
		buf[i] = '\0';
	}

	buff_index = 0;
}

void add() {
	buf[buff_index] = ch;
	buf[buff_index + 1] = '\0';
	buff_index++;
}

void makelex() {
	if (buff_index == 0)
		return;

	int indexInTable;
	if ((indexInTable = look(TW)) != NOT_FOUND) {
		makeInternalRep(TW.table_number, indexInTable);
	}
	else if (isConstant()) {
		if ((indexInTable = look(TNUM)) == NOT_FOUND) {
			indexInTable = putl(&TNUM);
		}
		makeInternalRep(TNUM.table_number, indexInTable);
	}
	else {
		if (!isLegalId())
			error(0);
		if ((indexInTable = look(TID)) == NOT_FOUND) {
			indexInTable = putl(&TID);
		}
		makeInternalRep(TID.table_number, indexInTable);
	}
}

void makeInternalRep(int tableNum, int numberInTable) {
	fprintf(output, "<%d, %d>", tableNum, numberInTable);
}

int isConstant() {
	int count = 0;
	for (int i = 0; i < buff_index; i++) {
		if (!isdigit(buf[i])) {
			if (buf[i] != '.')
				return 0;
			count++;
			if (count > 1)
				return 0;
		}
	}

	return 1;
}

void gc() {
	ch = fgetc(input);
}

int look(tabl t) {
	for (int i = 0; i < t.size; i++) {
		if (!strcmp(buf, t.table + i * t.word_size)) {
			return i;
		}
	}

	return NOT_FOUND;
}

int putl(tabl* t) {
	int index = t->size++;
	strcpy(t->table + index * t->word_size, buf);
	return t->size - 1;
}

int isLegalId() {
	if (!isalpha(buf[0]))
		return 0;

	for (int i = 1; i < buff_index; i++) {
		if (!isalnum(buf[i]))
			return 0;
	}

	return 1;
}

void tabl_init() {

	TW.table = (char *)TW_char;
	TW.size = SIZE_OF_TW_TABLE;
	TW.word_size = WORD_SIZE_IN_TABLE_TW;
	TW.table_number = 1;

	TD.table = (char *)TD_char;
	TD.size = SIZE_OF_TD_TABLE;
	TD.word_size = WORD_SIZE_IN_TABLE_TD;
	TD.table_number = 2;

	TNUM_char = (char**)malloc(sizeof(char*) * SIZE_OF_TNUM_TABLE);
	for (int i = 0; i < SIZE_OF_TNUM_TABLE; i++) {
		TNUM_char[i] = (char*)malloc(sizeof(char) * MAX_NUM_SIZE);
	}
	TNUM.table_r = (record *)malloc(sizeof(record) * SIZE_OF_TNUM_TABLE);

	TNUM.table = (char *)TNUM_char;
	TNUM.size = 0;
	TNUM.word_size = MAX_NUM_SIZE;
	TNUM.table_number = 3;

	TID_char = (char**)malloc(sizeof(char*) * SIZE_OF_TID_TABLE);
	for (int i = 0; i < SIZE_OF_TID_TABLE; i++) {
		TID_char[i] = (char*)malloc(sizeof(char) * MAX_ID_SIZE);
	}
	TID.table_r = (record *)malloc(sizeof(record) * SIZE_OF_TID_TABLE);

	TID.table = (char *)TID_char;
	TID.size = 0;
	TID.word_size = MAX_ID_SIZE;
	TID.table_number = 4;
}
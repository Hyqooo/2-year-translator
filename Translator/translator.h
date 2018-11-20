/*
	«аголовочный файл трансл€тора. —одержит таблицы, константы и описани€ функций
*/

#ifndef TRANSLATOR
#define TRANSLATOR

#define MAX_ID_SIZE 64
#define MAX_NUM_SIZE 15
#define SIZE_OF_TW_TABLE 20
#define SIZE_OF_TD_TABLE 13
#define SIZE_OF_TNUM_TABLE 64
#define SIZE_OF_TID_TABLE 128
#define BUFFER_SIZE 64
#define WORD_SIZE_IN_TABLE_TW 10
#define WORD_SIZE_IN_TABLE_TD 4
#define NOT_FOUND -1

//	table - указатель на необходимую таблицу.
//	size - размер таблицы, необходим при проходе через таблицу.
//	word_size - максимальный размер одного слова в таблице,
//		при проходе, при разной длине слов возникает путаница, указатель попадает
//		не на первую строку в таблице.
//	table_number - номер таблицы дл€ удобного вывода.

typedef struct tabl {
	char *table;
	int size;
	int word_size;
	int table_number;
}tabl;

/*
	ѕрототипы
*/
void clear();
void add();
void makelex();
void gc();
void tabl_init();
void tabl_free();
void error();
void lex_analyzer();
void makeInternalRep();
void delimiterParser();
void printTable();
int isConstant();
int isLegalId();
int look(tabl t);
int putl(tabl* t);
int putnum();

/*
	“аблицы
*/
// “аблица служебных слов.
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

// “аблица разделителей.
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

// “аблица идентификаторов и численных констант соответственно.
char **TID_char, **TNUM_char;
#endif // ! TRANSLATOR
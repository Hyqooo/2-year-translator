#ifndef LEXICAL
#define LEXICAL

/*
	 table - номер таблицы
	 numberInTable - номер лексемы в таблице
*/
typedef struct lex {
	int table;
	int numberInTable;
}lex;

/*
	table - указатель на необходимую таблицу.
	size - размер таблицы, необходим при проходе через таблицу.
	word_size - максимальный размер одного слова в таблице,
		при проходе, при разной длине слов возникает путаница, указатель попадает
		не на первую строку в таблице.
	table_number - номер таблицы для удобного вывода.
*/
typedef struct tabl {
	char *table;
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

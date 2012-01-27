%{

#include <stdio.h>
#include <stdlib.h>
	
%}

%union {
	int intVal;
	char *strVal;
	void* pVal;
}

%token SEMICOLON COLON LCB RCB PROGRAM FUNCTION BEGIN_BLOCK END_BLOCK FORWARD RETURN VAR IF WHILE DO OD VOID INT BOOLEAN ISET WRITE READ ADD_ISET REMOVE_ISET TO FROM CONSTANT 

%left AND OR
%left NOT
%left EQ LTE LT IN
%left PLUS MINUS
%left TIMES DIV
%left MIN_ISET MAX_ISET SIZE_ISET
%left LP RP

%type <pVal> Program

%%

Program : /**/ {$$=NULL;}

%%

int yyerror() {
	fprintf(stderr, "KO\n");
	fprintf(stderr, "Grammatical error.\n");
	exit(1);
}

int main() {
	yyparse();
}
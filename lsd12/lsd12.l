%{

#include "y.tab.h"

int num_line = 1;
%}

%option noyywrap

id 		[a-zA-Z][0-9a-zA-Z]*
blank		[ \t]
nbr             (0|[1-9][0-9]*)
comment		"{"(.*?)"}"

%%

";"		{return SEMICOLON;}
":"		{return COLON;}
","		{return COMMA;}

":="            {return ASSIGN;}
"+"		{return PLUS;}
"-"		{return MINUS;}
"/"		{return DIV;}
"*"		{return TIMES;}
"("		{return LP;}
")"		{return RP;}

"&&"		{return AND;}
"||"		{return OR;}
"!"			{return NOT;}
"="			{return EQ;}
"<="		{return LTE;}
"<"			{return LT;}
"in"		{return IN;}

"program"	{return PROGRAM;}
"function"	{return FUNCTION;}
"begin"		{return BEGIN_BLOCK;}
"end"		{return END_BLOCK;}
"forward"	{return FORWARD;}
"return"	{return RETURN;}
"var"		{return VAR;}
"if"		{return IF;}
"fi"		{return FI;}
"then"		{return THEN;}
"else"		{return ELSE;}
"while"		{return WHILE;}
"do"		{return DO;}
"od"		{return OD;}

"void"		{return VOID;}
"int"		{return INT;}
"bool"		{return BOOLEAN;}
"iset"		{return ISET;}

"write"		{return WRITE;}
"read"		{return READ;}

"#"			{return SIZE_ISET;}
"min"		{return MIN_ISET;}
"max"		{return MAX_ISET;}
"add"		{return ADD_ISET;}
"remove"	{return REMOVE_ISET;}
"to"		{return TO;}
"from"		{return FROM;}

"true"		{return BOOL_TRUE;}
"false"		{return BOOL_FALSE;}

{nbr} { 
	yylval.intVal = atoi(yytext); 
	return CONSTANT; 
}

{id} {
	yylval.strVal=(char*)calloc(strlen(yytext)+1,sizeof(char));
    strcpy(yylval.strVal,yytext);
    return ID;
}

{blank}	{}
"\n"	{
			++num_line; 
		}

{comment} {}

. {
	fprintf(stderr, "KO\n");
	fprintf(stderr, "Line: %d has an invalid character (%s)\n", num_line, yytext);
	exit(1);
}

%%
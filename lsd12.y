%{

#include <stdio.h>
#include <stdlib.h>

%}

%union {
	int intVal;
	char *strVal;
	void* pVal;
}

%token SEMICOLON COLON LCB RCB PROGRAM FUNCTION BEGIN_BLOCK END_BLOCK FORWARD RETURN VAR IF THEN ELSE FI WHILE DO OD VOID INT BOOLEAN ISET WRITE READ ADD_ISET REMOVE_ISET TO FROM CONSTANT BOOL_TRUE BOOL_FALSE COMMA NEW_LINE ID

%left AND OR
%left NOT
%left EQ LTE LT IN
%left ASSIGN
%left PLUS MINUS
%left TIMES DIV
%left MIN_ISET MAX_ISET SIZE_ISET
%left LP RP

%type <pVal> Program 

%%

Program : PROGRAM ID SEMICOLON 
		    Function 
		  END_BLOCK SEMICOLON
		  { $$ = 0; }
	;

Instruction : SEMICOLON
			| RExpr SEMICOLON
			| LExpr ASSIGN RExpr SEMICOLON
			| IF LP RExpr RP THEN InstructionList FI SEMICOLON
			| IF LP RExpr RP THEN InstructionList ELSE InstructionList FI SEMICOLON
			| WHILE LP RExpr RP DO InstructionList OD SEMICOLON
			| WRITE RExpr SEMICOLON
			| READ RExpr SEMICOLON
			| ADD_ISET IntExpr TO LExpr SEMICOLON
			| REMOVE_ISET IntExpr FROM LExpr SEMICOLON
			| RETURN RExpr
	;

InstructionList : /* nothing */
			    | InstructionList Instruction
    ;

TypeDecl : INT 
		 | BOOLEAN
		 | ISET
	;

Function : FUNCTION ID LP FunctionParams RP COLON FuncType SEMICOLON 
		 	  ImplementOrForward
	;

ImplementOrForward: FunctionBody 
				  | FORWARD SEMICOLON
	;

FuncType : INT
		 | BOOLEAN
		 | VOID
	;

FunctionParams : /* nothing */
			   | ParamDeclList
	;

ParamDecl : ID COLON TypeDecl
		  | VAR ID COLON TypeDecl
	;

ParamDeclList : ParamDecl
			  | ParamDeclList COMMA ParamDecl
	;

VarDecl : ID TypeDecl SEMICOLON
	;

FunctionBody : DeclBloc
               CodeBloc
    ;

FunctionDeclList: FunctionDeclList VarDecl
				| FunctionDeclList Function
				| VarDecl
				| Function
	;

DeclBloc : VAR FunctionDeclList
	;

CodeBloc : BEGIN_BLOCK InstructionList END_BLOCK SEMICOLON
	;

LExpr	: ID
	;

CallParams : /**/
		   | RExpr
		   | CallParams COMMA RExpr
	;

FunctionCall : ID LP CallParams RP
	;

RExpr :   LExpr
		| FunctionCall
		| CONSTANT
		| RExpr PLUS RExpr
		| RExpr MINUS RExpr
		| RExpr TIMES RExpr
		| RExpr DIV RExpr
		| MIN_ISET LExpr
		| MAX_ISET LExpr
		| SIZE_ISET LExpr
		| LP RExpr RP
		| BOOL_FALSE
		| BOOL_TRUE
		| RExpr AND RExpr
		| RExpr OR RExpr
		| NOT RExpr
		| RExpr EQ RExpr
		| RExpr LT RExpr
		| RExpr LTE RExpr
		| RExpr IN RExpr
	;

IntExpr : CONSTANT
		| IntExpr PLUS IntExpr
		| IntExpr MINUS IntExpr
		| IntExpr TIMES IntExpr
		| IntExpr DIV IntExpr
		| MIN_ISET LExpr
		| MAX_ISET LExpr
		| SIZE_ISET LExpr
		| LExpr
		| FunctionCall
		| LP IntExpr RP
	;

%%

int yyerror() {
	fprintf(stderr, "KO\n");
	fprintf(stderr, "Line %d: Grammatical error.\n", 1);
	exit(1);
}

int main() {
	yydebug = 1;
	yyparse();
}
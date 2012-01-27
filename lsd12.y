%{

#include <stdio.h>
#include <stdlib.h>

%}

%union {
	int intVal;
	char *strVal;
	void* pVal;
}

%token SEMICOLON COLON LCB RCB PROGRAM FUNCTION BEGIN_BLOCK END_BLOCK FORWARD RETURN VAR IF THEN ELSE FI WHILE DO OD VOID INT BOOLEAN ISET WRITE READ ADD_ISET REMOVE_ISET TO FROM CONSTANT BOOL_TRUE BOOL_FALSE COMMA ID NEW_LINE

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

Function : ID LP FunctionParams RP COLON FuncType SEMICOLON 
		 	  ImplementOrForward
	;

ImplementOrForward: FunctionBody | FORWARD SEMICOLON

FuncType : INT
		 | BOOLEAN
		 | VOID
	;

FunctionParams : VarDecl
	;

VarDecl : ID TypeDecl
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

LExpr	: ID
	;

RExpr 	: CONSTANT
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
		| BOOL_FALSE
		| BOOL_TRUE
		| BoolExpr AND BoolExpr
		| BoolExpr OR BoolExpr
		| NOT BoolExpr
		| IntExpr EQ IntExpr
		| IntExpr LT IntExpr
		| IntExpr LTE IntExpr
		| IntExpr IN LExpr
		| LP BoolExpr RP
	;

FunctionCall: ID LP RExpr RP
	;

IntExpr	: CONSTANT
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

BoolExpr: BOOL_FALSE
		| BOOL_TRUE
		| BoolExpr AND BoolExpr
		| BoolExpr OR BoolExpr
		| NOT BoolExpr
		| IntExpr EQ IntExpr
		| IntExpr LT IntExpr
		| IntExpr LTE IntExpr
		| IntExpr IN LExpr
		| LExpr
		| FunctionCall
		| LP BoolExpr RP
	;

%%

int yyerror() {
	fprintf(stderr, "KO\n");
	fprintf(stderr, "Line %d: Grammatical error.\n", 1);
	exit(1);
}

int main() {
	yyparse();
}
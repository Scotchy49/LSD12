%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "sym.h"

AST_TREE root = 0;

%}

%union {
	int intVal;
	char *strVal;
	struct ast_node* ast;
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

%type <ast> Program Function Instruction InstructionList TypeDecl FuncType FunctionParams ParamDecl VarDecl DeclBloc CodeBloc FunctionCall LExpr CallParams RExpr
%type <strVal> ID
%type <intVal> CONSTANT
%%

Program : PROGRAM ID SEMICOLON Function END_BLOCK SEMICOLON { root = createNode(OP_PROGRAM, 2, createLiteral(OP_ID, $2), $4); }
	;

Instruction : SEMICOLON 																{/*no side effect*/}
			| RExpr SEMICOLON															{$$=$1;}
			| LExpr ASSIGN RExpr SEMICOLON 												{$$=createNode(OP_ASSIGN, 2, $1, $3);}
			| IF LP RExpr RP THEN InstructionList FI SEMICOLON 							{$$=createNode(OP_IF, 2, $3, $6);}
			| IF LP RExpr RP THEN InstructionList ELSE InstructionList FI SEMICOLON 	{$$=createNode(OP_IF_ELSE, 3, $3, $6, $8);}
			| WHILE LP RExpr RP DO InstructionList OD SEMICOLON 						{$$=createNode(OP_WHILE, 2, $3, $6);}
			| WRITE RExpr SEMICOLON 													{$$=createNode(OP_WRITE, 1, $2);}
			| READ LExpr SEMICOLON 														{$$=createNode(OP_READ, 1, $2);}
			| ADD_ISET RExpr TO LExpr SEMICOLON 										{$$=createNode(OP_ADD_ISET, 2, $2, $4);}
			| REMOVE_ISET RExpr FROM LExpr SEMICOLON 									{$$=createNode(OP_REMOVE_ISET, 2, $2, $4);}
			| RETURN RExpr SEMICOLON 													{$$=createNode(OP_RETURN, 1, $2);}
	;

InstructionList : /* nothing */ 				{$$=createNode(OP_INSTRUCTION_LIST, 0);}
			    | InstructionList Instruction 	{$$=addChildNode($1, $2);}
    ;

TypeDecl : INT  		{$$=createIntConstant(OP_VAR_TYPE, TYPE_INT);}
		 | BOOLEAN 		{$$=createIntConstant(OP_VAR_TYPE, TYPE_BOOLEAN);}
		 | ISET 		{$$=createIntConstant(OP_VAR_TYPE, TYPE_ISET);}
	;

Function : FUNCTION ID LP FunctionParams RP COLON FuncType SEMICOLON DeclBloc CodeBloc	{$$=createNode(OP_FUNCTION, 5, createLiteral(OP_ID, $2), $4, $7, $9, $10);}
		 | FUNCTION ID LP FunctionParams RP COLON FuncType SEMICOLON FORWARD SEMICOLON 	{$$=createNode(OP_FUNCTION_FORWARD, 3, createLiteral(OP_ID, $2), $4, $7);}
	;

FuncType : INT 				{$$=createIntConstant(OP_FUNCTION_TYPE, TYPE_INT);}
		 | BOOLEAN			{$$=createIntConstant(OP_FUNCTION_TYPE, TYPE_BOOLEAN);}
		 | VOID 			{$$=createIntConstant(OP_FUNCTION_TYPE, TYPE_VOID);}
	;

FunctionParams : /* nothing */ 					{$$=createNode(OP_FUNCTION_PARAMS, 0);}
			   | ParamDecl 						{$$=createNode(OP_FUNCTION_PARAMS, 1, $1);}
			   | FunctionParams COMMA ParamDecl	{$$=addChildNode($1, $3);}
	;

ParamDecl : ID COLON TypeDecl 		
				{$$=createNode(OP_FUNCTION_PARAM, 3, createIntConstant(OP_FUNCTION_PARAM_VAR, 0), createLiteral(OP_ID, $1), $3);}
		  | VAR ID COLON TypeDecl 	
		  		{$$=createNode(OP_FUNCTION_PARAM, 3, createIntConstant(OP_FUNCTION_PARAM_VAR, 1), createLiteral(OP_ID, $2), $4);}
	;

VarDecl : ID TypeDecl SEMICOLON 	{$$=createNode(OP_FUNCTION_VAR_DECL, 2, createLiteral(OP_ID, $1), $2);}
	;

DeclBloc : VAR VarDecl		{$$=createNode(OP_FUNCTION_DECLBLOCK, 1, $2);}
		 | VAR Function		{$$=createNode(OP_FUNCTION_DECLBLOCK, 1, $2);}
		 | DeclBloc VarDecl {$$=addChildNode($1, $2);}
		 | DeclBloc Function {$$=addChildNode($1, $2);}
	;

CodeBloc : BEGIN_BLOCK InstructionList END_BLOCK SEMICOLON 	{$$=createNode(OP_FUNCTION_BODY, 1, $2);}
	;

FunctionCall : ID LP CallParams RP 	{$$=createNode(OP_FUNCTION_CALL, 2, createLiteral(OP_ID,$1), $3);}
	;

LExpr	: ID 	{$$ = createLiteral(OP_LEXPR, $1);}
	;

CallParams : /**/ 						{$$=createNode(OP_FUNCTION_CALL_PARAMS, 0);}
		   | RExpr 						{$$=createNode(OP_FUNCTION_CALL_PARAMS, 1, $1);}
		   | CallParams COMMA RExpr    	{$$=addChildNode($1, $3);}
	;

RExpr :   LExpr 				{$$ = $1;}
		| LP RExpr RP 			{$$ = $2;}
		| FunctionCall			{$$ = $1;}
		| CONSTANT 				{$$ = createIntConstant(OP_CONSTANT_INT, $1);}
		| RExpr PLUS RExpr		{$$ = createNode(OP_PLUS, 2, $1, $3);}
		| RExpr MINUS RExpr		{$$ = createNode(OP_MINUS, 2, $1, $3);}
		| RExpr TIMES RExpr		{$$ = createNode(OP_TIMES, 2, $1, $3);}
		| RExpr DIV RExpr		{$$ = createNode(OP_DIV, 2, $1, $3);}
		| MIN_ISET LExpr		{$$ = createNode(OP_MIN_ISET, 1, $2);}
		| MAX_ISET LExpr		{$$ = createNode(OP_MAX_ISET, 1, $2);}
		| SIZE_ISET LExpr		{$$ = createNode(OP_SIZE_ISET, 1, $2);}
		| BOOL_FALSE			{$$ = createIntConstant(OP_CONSTANT_BOOL, 0);}
		| BOOL_TRUE				{$$ = createIntConstant(OP_CONSTANT_BOOL, 1);}
		| RExpr AND RExpr		{$$ = createNode(OP_AND, 2, $1, $3);}
		| RExpr OR RExpr		{$$ = createNode(OP_OR, 2, $1, $3);}
		| NOT RExpr				{$$ = createNode(OP_NOT, 1, $2);}
		| RExpr EQ RExpr		{$$ = createNode(OP_EQ, 2, $1, $3);}
		| RExpr LT RExpr		{$$ = createNode(OP_LT, 2, $1, $3);}
		| RExpr LTE RExpr		{$$ = createNode(OP_LTE, 2, $1, $3);}
		| RExpr IN LExpr		{$$ = createNode(OP_IN, 2, $1, $3);}
	;
%%

int yyerror() {
	fprintf(stderr, "KO\n");
	fprintf(stderr, "Line %d: Grammatical error.\n", 1);
	exit(1);
}

void printTree(AST_TREE node, int depth) {
	if( node != NULL ) {
		int i;
		for( i = 0; i < depth; ++i ) 
			printf("%s", "|  ");
		char * nodeStr = humanReadableNode(node);
		printf("%s\n", nodeStr);
		free(nodeStr);
		for( i = 0; i < node->op_count; ++i ) {
			printTree(node->operands[i], depth+1);
		}
	}
}

int main() {
	yyparse();
	printTree(root, 0);
	
	
	freeTree(root);
	return 0;
}

#ifndef AST_H
#define AST_H

typedef enum {
	OP_REXPR,
	OP_ID,
	OP_LEXPR,
	OP_CONSTANT_INT,
	OP_CONSTANT_BOOL,
	OP_VAR_TYPE,

	OP_PROGRAM,
	
	OP_INSTRUCTION,
	OP_INSTRUCTION_LIST,
	OP_ASSIGN,
	OP_IF,
	OP_IF_ELSE,
	OP_WHILE,
	OP_WRITE,
	OP_READ,
	OP_ADD_ISET,
	OP_REMOVE_ISET,
	OP_RETURN,
	
	OP_FUNCTION,			
	OP_FUNCTION_PROTOTYPE,
	OP_FUNCTION_TYPE,
	OP_FUNCTION_PARAM,
	OP_FUNCTION_PARAMS,
	OP_FUNCTION_PARAM_VAR,
	OP_FUNCTION_FORWARD,
	
	OP_FUNCTION_DECLBLOCK,
	OP_FUNCTION_VAR_DECL,
	OP_FUNCTION_FUNCTION_DECL,
	OP_FUNCTION_BODY,
	
	OP_FUNCTION_CALL,
	OP_FUNCTION_CALL_PARAMS,
	
	OP_PLUS,
	OP_MINUS,
	OP_TIMES,
	OP_DIV,
	OP_MIN_ISET,
	OP_MAX_ISET,
	OP_SIZE_ISET,
	OP_AND,
	OP_OR,
	OP_NOT,
	OP_EQ,
	OP_LT,
	OP_LTE,
	OP_IN
} OP_TYPE;

typedef enum {
	TYPE_INT,
	TYPE_BOOLEAN,
	TYPE_VOID,
	TYPE_ISET
} VAR_TYPE;

typedef struct ast_node {
	OP_TYPE type;
	int op_count;

	struct ast_node **operands;
	int intVal;
	char *strVal;
} AST_NODE, *AST_TREE;

char *getVarTypeName(int val);
char *humanReadableNode(AST_TREE node);
AST_TREE createLiteral(OP_TYPE type, char* literal);
AST_TREE createIntConstant( OP_TYPE type, int value );
AST_TREE createNode( OP_TYPE type, int opCount, ... );
AST_TREE addChildNode( AST_TREE parent, AST_TREE child );
void freeTree( AST_TREE tree );

#endif

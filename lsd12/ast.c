/*
 * ast.c
 *
 *  Created on: Jan 29, 2012
 *      Author: scotchy
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "ast.h"
#include "sym.h"

extern int num_line;

char *getVarTypeName(int val) {
    switch(val) {
    case TYPE_INT:
        return "i";
    case TYPE_BOOLEAN:
        return "b";
    case TYPE_VOID:
        return "v";
    case TYPE_ISET:
        return "a";
    default:
        return "unknown";    
    }
}

char *humanReadableNode(AST_TREE node) {
    char *str = malloc(511);
    OP_TYPE type = node->type;
    switch(type) {
    case OP_REXPR:
        sprintf( str, "OP_REXPR");
        break;
    case OP_ID:
        sprintf( str, "OP_ID (%s)", node->strVal);
        break;
    case OP_LEXPR:
        sprintf( str, "OP_LEXPR (%s)", node->strVal);
        break;
    case OP_CONSTANT_INT:
        sprintf( str, "OP_CONSTANT_INT (%d)", node->intVal);
        break;
    case OP_CONSTANT_BOOL:
        sprintf( str, "OP_CONSTANT_BOOL (%d)", node->intVal);
        break;
    case OP_VAR_TYPE:
        sprintf( str, "OP_VAR_TYPE (%s)", getVarTypeName(node->intVal));
        break;
    case OP_PROGRAM:
        sprintf( str, "OP_PROGRAM");
        break;
    case OP_INSTRUCTION:
        sprintf( str, "OP_INSTRUCTION");
        break;
    case OP_INSTRUCTION_LIST:
        sprintf( str, "OP_INSTRUCTION_LIST");
        break;
    case OP_ASSIGN:
        sprintf( str, "OP_ASSIGN");
        break;
    case OP_IF:
        sprintf( str, "OP_IF");
        break;
    case OP_IF_ELSE:
        sprintf( str, "OP_IF_ELSE");
        break;
    case OP_WHILE:
        sprintf( str, "OP_WHILE");
        break;
    case OP_WRITE:
        sprintf( str, "OP_WRITE");
        break;
    case OP_READ:
        sprintf( str, "OP_READ");
        break;
    case OP_ADD_ISET:
        sprintf( str, "OP_ADD_ISET");
        break;
    case OP_REMOVE_ISET:
        sprintf( str, "OP_REMOVE_ISET");
        break;
    case OP_RETURN:
        sprintf( str, "OP_RETURN");
        break;
    case OP_FUNCTION:
        sprintf( str, "OP_FUNCTION");
        break;
    case OP_FUNCTION_PROTOTYPE:
        sprintf( str, "OP_FUNCTION_PROTOTYPE");
        break;
    case OP_FUNCTION_TYPE:
        sprintf( str, "OP_FUNCTION_TYPE (%s)", getVarTypeName(node->intVal));
        break;
    case OP_FUNCTION_PARAM:
        sprintf( str, "OP_FUNCTION_PARAM");
        break;
    case OP_FUNCTION_PARAMS:
        sprintf( str, "OP_FUNCTION_PARAMS");
        break;
    case OP_FUNCTION_PARAM_VAR:
        sprintf( str, "OP_FUNCTION_PARAM_VAR (%d)", node->intVal);
        break;
    case OP_FUNCTION_FORWARD:
        sprintf( str, "OP_FUNCTION_FORWARD");
        break;
    case OP_FUNCTION_DECLBLOCK:
        sprintf( str, "OP_FUNCTION_DECLBLOCK");
        break;
    case OP_FUNCTION_VAR_DECL:
        sprintf( str, "OP_FUNCTION_VAR_DECL");
        break;
    case OP_FUNCTION_FUNCTION_DECL:
        sprintf( str, "OP_FUNCTION_FUNCTION_DECL");
        break;
    case OP_FUNCTION_BODY:
        sprintf( str, "OP_FUNCTION_BODY");
        break;
    case OP_FUNCTION_CALL:
        sprintf( str, "OP_FUNCTION_CALL");
        break;
    case OP_FUNCTION_CALL_PARAM:
        sprintf( str, "OP_FUNCTION_CALL_PARAM");
        break;
    case OP_FUNCTION_CALL_PARAMS:
        sprintf( str, "OP_FUNCTION_CALL_PARAMS");
        break;
    case OP_NEG:
        sprintf( str, "OP_NEG" );
        break;
    case OP_PLUS:
        sprintf( str, "OP_PLUS");
        break;
    case OP_MINUS:
        sprintf( str, "OP_MINUS");
        break;
    case OP_TIMES:
        sprintf( str, "OP_TIMES");
        break;
    case OP_DIV:
        sprintf( str, "OP_DIV");
        break;
    case OP_MIN_ISET:
        sprintf( str, "OP_MIN_ISET");
        break;
    case OP_MAX_ISET:
        sprintf( str, "OP_MAX_ISET");
        break;
    case OP_SIZE_ISET:
        sprintf( str, "OP_SIZE_ISET");
        break;
    case OP_AND:
        sprintf( str, "OP_AND");
        break;
    case OP_OR:
        sprintf( str, "OP_OR");
        break;
    case OP_NOT:
        sprintf( str, "OP_NOT");
        break;
    case OP_EQ:
        sprintf( str, "OP_EQ");
        break;
    case OP_LT:
        sprintf( str, "OP_LT");
        break;
    case OP_LTE:
        sprintf( str, "OP_LTE");
        break;
    case OP_IN:
        sprintf( str, "OP_IN");
        break;
    default:
        sprintf(str, "%d", type);
        break;
    }

    sprintf(str, "%s [%s]",str, printSymbols(node->symbols));

    return str;
}

AST_TREE initNode(OP_TYPE type) {
    AST_TREE n = malloc(sizeof *n);
    if( !n ) {
        fprintf(stderr, "KO\n");
        fprintf(stderr, "Could not allocate enough memory !");
    }
    n->num_line = num_line;
    n->type = type;
    n->strVal = NULL;
    n->operands = NULL;
    n->next = NULL;
    n->symbols = NULL;
    n->parent = NULL;
    return n;
}

AST_TREE createLiteral(OP_TYPE type, char* literal) {
    AST_TREE n = initNode(type);
    n->strVal = literal;
    return n;
}

AST_TREE createIntConstant( OP_TYPE type, int value ) {
    AST_TREE n = initNode(type);
    n->intVal = value;
    return n;
}

AST_TREE createNode( OP_TYPE type, int opCount, ... ) {
    AST_TREE n = initNode(type);
    AST_TREE operandList = n->operands;

    va_list ap;
    int i;
    va_start(ap, opCount);    
    for( i = 0; i < opCount; ++i ) {
        AST_TREE nOperand = va_arg(ap, AST_TREE);
        if( nOperand ) {
            
            // assign the parent to the operands
            AST_TREE tmp = nOperand;
            while(tmp) {
                tmp->parent = n;
                tmp = tmp->next;
            }
           
            // if this is a valid operand, we append it to the list
            if( operandList )
                operandList->next = nOperand;

            // we move the list forward
            operandList = nOperand;

            // if the operand list is still empty, we assign the first child to the current
            // node being created
            if( n->operands == NULL ) 
                n->operands = operandList;
            
            
        }
    }
    va_end(ap);
    return n;
}

AST_TREE getNodeOperand(AST_TREE node, OP_TYPE operand) {
    AST_TREE node_operand = node->operands;
    while(node_operand) {
        if( node_operand->type == operand ) 
            return node_operand;
        node_operand = node_operand->next;
    }
    return NULL;
}

/*
 * puts child at the end of parent
 */
AST_TREE addSiblingNode( AST_TREE elder, AST_TREE younger ) {
    AST_TREE end = elder;
    while( end->next != NULL )
        end = end->next;

    end->next = younger;
    if( younger )
        younger->parent = end->parent;
    return elder;
}

void freeTree( AST_TREE tree ) {
    
}

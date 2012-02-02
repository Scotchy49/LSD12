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


char *getVarTypeName(int val) {
    switch(val) {
    case TYPE_INT:
        return "i";
    case TYPE_BOOLEAN:
        return "b";
    case TYPE_VOID:
        return "v";
    case TYPE_ISET:
        return "is";
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
        sprintf( str, "OP_INSTRUCTION_LIST (%d)", node->op_count);
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
        sprintf( str, "OP_OP");
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

    sprintf(str, "%s [%s]", str, printSymbols(node->symbols));

    return str;
}

AST_TREE initNode(OP_TYPE type) {
    AST_TREE newNode = malloc(sizeof(AST_NODE));
    newNode->type = type;
    newNode->op_count = 0;
    newNode->strVal = NULL;
    newNode->operands = NULL;
    newNode->next = NULL;
    newNode->symbols = NULL;
    return newNode;
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
    n->op_count = opCount;
    n->operands = calloc(opCount, sizeof *n->operands);

    va_list ap;
    int i;
    va_start(ap, opCount);    
    for (i = 0; i < opCount; i++) {
        n->operands[i] = va_arg(ap, AST_TREE);
    }
    va_end(ap);
    return n;
}

AST_TREE getNodeOperand(AST_TREE node, OP_TYPE operand) {
    int i;
    for( i=0; i < node->op_count; ++i ) {
        if( node->operands[i] && node->operands[i]->type == operand )
            return node->operands[i];
    }
}

/*
 * puts child at the end of parent
 */
AST_TREE addChildNode( AST_TREE parent, AST_TREE child ) {
    AST_TREE end = parent;
    while( end->next != NULL )
        end = end->next;

    end->next = child;
    return parent;
}

void freeTree( AST_TREE tree ) {
    if( tree != NULL ) {
        free(tree->strVal);
        int i;
        for( i = 0; i < tree->op_count; ++i ) {
            freeTree(tree->operands[i]);
        }
    }
    free(tree);
}

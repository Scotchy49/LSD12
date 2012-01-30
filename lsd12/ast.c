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


char *getVarTypeName(int val) {
    switch(val) {
    case TYPE_INT:
        return "int";
    case TYPE_BOOLEAN:
        return "bool";
    case TYPE_VOID:
        return "void";
    case TYPE_ISET:
        return "iset";
    }
}

char *humanReadableNode(AST_TREE node) {
    char *str = malloc(sizeof(char)*255);
    OP_TYPE type = node->type;
    switch(type) {
    case OP_REXPR:
        sprintf( str, "OP_REXPR");
        return str;
    case OP_ID:
        sprintf( str, "OP_ID (%s)", node->strVal);
        return str;
    case OP_LEXPR:
        sprintf( str, "OP_LEXPR (%s)", node->strVal);
        return str;
    case OP_CONSTANT_INT:
        sprintf( str, "OP_CONSTANT_INT (%d)", node->intVal);
        return str;
    case OP_CONSTANT_BOOL:
        sprintf( str, "OP_CONSTANT_BOOL (%d)", node->intVal);
        return str;
    case OP_VAR_TYPE:
        sprintf( str, "OP_VAR_TYPE (%s)", getVarTypeName(node->intVal));
        return str;
    case OP_PROGRAM:
        sprintf( str, "OP_PROGRAM");
        return str;
    case OP_INSTRUCTION:
        sprintf( str, "OP_INSTRUCTION");
        return str;
    case OP_INSTRUCTION_LIST:
        sprintf( str, "OP_INSTRUCTION_LIST (%d)", node->op_count);
        return str;
    case OP_ASSIGN:
        sprintf( str, "OP_ASSIGN");
        return str;
    case OP_IF:
        sprintf( str, "OP_IF");
        return str;
    case OP_IF_ELSE:
        sprintf( str, "OP_IF_ELSE");
        return str;
    case OP_WHILE:
        sprintf( str, "OP_WHILE");
        return str;
    case OP_WRITE:
        sprintf( str, "OP_WRITE");
        return str;
    case OP_READ:
        sprintf( str, "OP_READ");
        return str;
    case OP_ADD_ISET:
        sprintf( str, "OP_ADD_ISET");
        return str;
    case OP_REMOVE_ISET:
        sprintf( str, "OP_REMOVE_ISET");
        return str;
    case OP_RETURN:
        sprintf( str, "OP_RETURN");
        return str;
    case OP_FUNCTION:
        sprintf( str, "OP_FUNCTION");
        return str;
    case OP_FUNCTION_PROTOTYPE:
        sprintf( str, "OP_FUNCTION_PROTOTYPE");
        return str;
    case OP_FUNCTION_TYPE:
        sprintf( str, "OP_FUNCTION_TYPE (%s)", getVarTypeName(node->intVal));
        return str;
    case OP_FUNCTION_PARAM:
        sprintf( str, "OP_FUNCTION_PARAM");
        return str;
    case OP_FUNCTION_PARAMS:
        sprintf( str, "OP_FUNCTION_PARAMS");
        return str;
    case OP_FUNCTION_PARAM_VAR:
        sprintf( str, "OP_FUNCTION_PARAM_VAR (%d)", node->intVal);
        return str;
    case OP_FUNCTION_FORWARD:
        sprintf( str, "OP_FUNCTION_FORWARD");
        return str;
    case OP_FUNCTION_DECLBLOCK:
        sprintf( str, "OP_FUNCTION_DECLBLOCK");
        return str;
    case OP_FUNCTION_VAR_DECL:
        sprintf( str, "OP_FUNCTION_VAR_DECL");
        return str;
    case OP_FUNCTION_FUNCTION_DECL:
        sprintf( str, "OP_FUNCTION_FUNCTION_DECL");
        return str;
    case OP_FUNCTION_BODY:
        sprintf( str, "OP_FUNCTION_BODY");
        return str;
    case OP_FUNCTION_CALL:
        sprintf( str, "OP_FUNCTION_CALL");
        return str;
    case OP_FUNCTION_CALL_PARAM:
        sprintf( str, "OP_FUNCTION_CALL_PARAM");
        return str;
    case OP_FUNCTION_CALL_PARAMS:
        sprintf( str, "OP_FUNCTION_CALL_PARAMS");
        return str;
    case OP_PLUS:
        sprintf( str, "OP_PLUS");
        return str;
    case OP_MINUS:
        sprintf( str, "OP_MINUS");
        return str;
    case OP_TIMES:
        sprintf( str, "OP_TIMES");
        return str;
    case OP_DIV:
        sprintf( str, "OP_DIV");
        return str;
    case OP_MIN_ISET:
        sprintf( str, "OP_MIN_ISET");
        return str;
    case OP_MAX_ISET:
        sprintf( str, "OP_MAX_ISET");
        return str;
    case OP_SIZE_ISET:
        sprintf( str, "OP_SIZE_ISET");
        return str;
    case OP_AND:
        sprintf( str, "OP_AND");
        return str;
    case OP_OR:
        sprintf( str, "OP_OP");
        return str;
    case OP_NOT:
        sprintf( str, "OP_NOT");
        return str;
    case OP_EQ:
        sprintf( str, "OP_EQ");
        return str;
    case OP_LT:
        sprintf( str, "OP_LT");
        return str;
    case OP_LTE:
        sprintf( str, "OP_LTE");
        return str;
    case OP_IN:
        sprintf( str, "OP_IN");
        return str;
    default:
        sprintf(str, "%d", type);
        return str;
    }
}

AST_TREE createLiteral(OP_TYPE type, char* literal) {
    AST_TREE newNode = malloc(sizeof(AST_NODE));
    newNode->type = type;
    newNode->op_count = 0;
    newNode->strVal = literal;
    newNode->operands = NULL;
    newNode->next = NULL;
    return newNode;
}

AST_TREE createIntConstant( OP_TYPE type, int value ) {
    AST_TREE newNode = malloc(sizeof(AST_NODE));
    newNode->type = type;
    newNode->op_count = 0;

    newNode->intVal = value;

    newNode->strVal = NULL;
    newNode->operands = NULL;
    newNode->next = NULL;
    return newNode;
}

AST_TREE createNode( OP_TYPE type, int opCount, ... ) {
    AST_TREE newNode = malloc(sizeof(AST_NODE));
    newNode->type = type;
    newNode->strVal = NULL;
    newNode->next = NULL;

    newNode->op_count = opCount;
    newNode->operands = calloc(opCount, sizeof *newNode->operands);
    int i;
    va_list ap;
    va_start(ap, opCount);
    for (i = 0; i < opCount; i++)
        newNode->operands[i] = va_arg(ap, AST_TREE);
    va_end(ap);
    return newNode;
}

AST_TREE addChildNode( AST_TREE parent, AST_TREE child ) {
    AST_TREE end = parent;
    if( end->next != NULL )
        end = parent->next;

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

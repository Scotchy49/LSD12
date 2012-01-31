
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "sym.h"
/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol ) {
    if( symbol ) {
        symbol->next = list;
        return symbol;
    }
    return list;
}

SYMLIST createSymbol(char *id, int type) {
    SYMLIST s = malloc(sizeof *s);
    s->id = id;
    s->pos = 0;
    s->depth = 0;
    s->next = NULL;
    return s;
}

SYMLIST getFunctionSymbol(AST_TREE node) {
    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_FUNCTION_TYPE)->intVal;
    // todo: handle params

    return createSymbol(id, type);
}

SYMLIST getVarSymbol(AST_TREE node) {

    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_VAR_TYPE)->intVal;

    return createSymbol(id, type);
}

SYMLIST getSymbol(AST_TREE node, int depth) {
    int i;
    SYMLIST sym = NULL;

    if(node->type == OP_FUNCTION || node->type == OP_FUNCTION_FORWARD ) {
        sym = getFunctionSymbol(node);
    } else if (node->type == OP_FUNCTION_VAR_DECL) {
        sym = getVarSymbol(node);
    } else if(node->type == OP_FUNCTION_PARAM ) {
        sym = getVarSymbol(node);
    }

    if( sym ) {
        sym->depth = depth;
    }

    return sym;
}

char *printSymbols(SYMLIST s) {
    char *symbols = malloc(255);
    while(s) {
        sprintf(symbols, "%s -> %s (%d)", symbols, s->id, s->depth);
        s = s->next;
    }
    return symbols;
}


void popSymbols(AST_TREE root) {
    if( root->type == OP_FUNCTION_PARAMS ) {
        AST_TREE paramList = root->operands[0];
        if( paramList ) {
            while(paramList->next) {
                paramList = paramList->next;
            }
            root->symbols = paramList->symbols;
        }

    } else if( root->type == OP_FUNCTION_DECLBLOCK ) {
        AST_TREE declList = root->operands[0];
        while(declList->next) {
            declList = declList->next;
        }
        root->symbols = declList->symbols;
    }
}

void populateSymbols( AST_TREE root, SYMLIST accessible, int depth ) {
    int i;

    if( root ) {
        // each node inherits the symbols of its father, so we prepend the current node's
        // symbol with the "already" accessible ones
        root->symbols = prependSymbol(accessible, getSymbol(root, depth));

        // the operands inherit from the operator's symbols, so we will pass those to them
        // while populating them. As we give symbols to the operands, we will discover new
        // symbols. The successors will inherit those.
        SYMLIST tmpSymbols = root->symbols;
        for(i = 0; i < root->op_count; ++i) {
            AST_TREE operand = root->operands[i];
            if( operand ) {

                // depth is relative to function, so we increase the depth only if we traversed a function operator
                int d = depth;
                if( root->type == OP_FUNCTION ) {
                    d++;
                }

                populateSymbols(operand, tmpSymbols, d);
                tmpSymbols = operand->symbols;
            }
        }

        // once we populated the operands, sometimes we need to bubble back new identifiers
        // back to the operator.
        popSymbols(root);

        // the next vertical operators inherit automatically from this node, too
        populateSymbols(root->next, root->symbols, depth);
    }
}

void fillSymbols( AST_TREE program ) {

    // on créer les symboles
    populateSymbols(program, NULL, -1);

}

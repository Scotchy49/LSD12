
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "sym.h"

char *printSymbol(SYMLIST s) {
    char *symbol = malloc(255);
    char *fct = "";
    if( s->isFunction ) {
        fct = malloc(127);
        strcpy(fct, "(");
        SYMLIST params = s->paramList;
        while(params) {
            sprintf(fct, "%s%s%s", fct, getVarTypeName(s->type), params->next?",":"");
            params = params->next;
        }
        strcat(fct, ")");
    }

    sprintf(symbol, "%s%s%s:%s(%d)", s->isForward ? "-":"", s->id, fct, getVarTypeName(s->type), s->depth);
    return symbol;
}

int sameSymbols(SYMLIST a, SYMLIST b) {
    // they have to be on the same depth
    // other is it a shadow
    if( a->depth != b->depth )
        return 0;

    // ID check
    if( strcmp(a->id, b->id) != 0)
        return 0;

    // for functions, we check if the length and type of the operators are the same
    if( a->isFunction && b->isFunction ) {
        SYMLIST p_a = a->paramList;
        SYMLIST p_b = b->paramList;
        while(p_a->next || p_b->next) {
            if( !p_a->next || !p_b->next ) {
                // not the same number of params
                return 0;
            }

            // not the same type
            if( p_a->type != p_b->type )
                return 0;

            p_a = p_a->next;
            p_b = p_b->next;
        }

        if( a->isForward && !b->isForward )
            return 0;
    }

    return 1;
}

/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol ) {
    if( symbol ) {
        SYMLIST tmp = list;
        // check name constraints
        while( tmp && tmp->depth == symbol->depth ) {
            if(sameSymbols(tmp, symbol)) {
                fprintf(stderr, "KO\n");
                fprintf(stderr, "%s already defined\n", printSymbol(symbol));
                exit(1);
            }
            tmp = tmp->next;
        }

        symbol->next = list;
        return symbol;
    }
    return list;
}

SYMLIST createSymbol(char *id, int type) {
    SYMLIST s = malloc(sizeof *s);
    s->id = id;
    s->type = type;
    s->pos = 0;
    s->depth = 0;
    s->next = NULL;
    s->isFunction = 0;
    s->isForward = 0;
    return s;
}

SYMLIST getVarSymbol(AST_TREE node) {

    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_VAR_TYPE)->intVal;

    return createSymbol(id, type);
}

SYMLIST getFunctionSymbol(AST_TREE node) {
    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_FUNCTION_TYPE)->intVal;

    SYMLIST s = createSymbol(id, type);
    s->isFunction = 1;
    s->isForward = node->type == OP_FUNCTION_FORWARD;

    AST_TREE param = getNodeOperand(node, OP_FUNCTION_PARAMS)->operands[0];
    while(param) {
        s->paramList = prependSymbol(s->paramList, getVarSymbol(param));
        param = param->next;
    }
    return s;
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
    char *symbols = malloc(1024);
    while(s) {
        sprintf(symbols, "%s -> %s", symbols, printSymbol(s));
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

void populateSymbols( AST_TREE root, SYMLIST inherited, int depth ) {
    int i;

    if( root ) {
        // each node inherits the symbols of its father, so we prepend the current node's
        // symbol with the "already" accessible ones
        root->symbols = prependSymbol(inherited, getSymbol(root, depth));

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

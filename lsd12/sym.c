
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "sym.h"

SYMLIST appendSymbol( SYMLIST list, SYMLIST symbol ) {
    SYMLIST tmp = list;
    while(tmp->next) {
        tmp = tmp->next;
    }

    tmp->next = symbol;
    return list;
}

/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol ) {
    symbol->next = list;
    return symbol;
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

SYMLIST getSymbols(AST_TREE node, SYMLIST accessible, int depth ) {
    int i;
    SYMLIST s = accessible;

    if(node->type == OP_FUNCTION || node->type == OP_FUNCTION_FORWARD ) {
        SYMLIST fctSym = getFunctionSymbol(node);
        s = prependSymbol(s, fctSym);
        printf("fct %s d: %d q: %d\n", fctSym->id, depth, 0);
    } else if (node->type == OP_FUNCTION_VAR_DECL) {
        SYMLIST varSym = getVarSymbol(node);
        printf("symbol %s d: %d q: %d\n", varSym->id, depth, 0);
        s = prependSymbol(s, varSym);
    } else if( node->type == OP_FUNCTION_DECLBLOCK ) {
        /*AST_TREE vars = node->operands[0];
        while(vars) {
            s = getSymbols(vars, s, depth);
            vars = vars->next;
        }*/
    }

    return s;
}

char *printSymbols(SYMLIST s) {
    while(s) {
        printf("%s -> ", s->id);
        s = s->next;
    }
    printf("\n");
}

/*
 * fills root and its children with the symbols list
 * accessible represents the upward symbols (with depth = depth-1)
 * Returns the symbols accessible for root's siblings (downwards)
 */
SYMLIST fillSymbols( AST_TREE root, SYMLIST accessible, int depth ) {
    SYMLIST s = accessible;
    int i;

    // propagation verticale
    while(root) {
        s = getSymbols(root, s, depth);
        printf("for %s : ", humanReadableNode(root));
        printSymbols(s);
        root->symbols = s;

        // propagation horizontale
        for( i = 0; i < root->op_count; ++i ) {
            AST_TREE subNode = root->operands[i];
            if( subNode ) {
                fillSymbols(subNode, s, depth+1);
            }
        }

        root = root->next;
    }


    return s;
}

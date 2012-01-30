/*
 * main.c
 *
 *  Created on: Jan 29, 2012
 *      Author: scotchy
 */
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "ast.h"
#include "sym.h"

AST_TREE root = 0;

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

        if( node->next != NULL ) {
            for( i = 0; i < depth; ++i )
                printf("%s", "|  ");
            printf("next\n");
            printTree(node->next, depth);
        }
    }
}

extern int yydebug;

int main() {
    yydebug = 1;
    yyparse();
    printTree(root, 0);

    fillSymbols(getNodeOperand(root, OP_FUNCTION), NULL, 0);

    freeTree(root);
    return 0;
}

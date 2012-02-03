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

extern int num_line;

int yyerror(const char *error) {
    fprintf(stderr, "KO\n");
    fprintf(stderr, "Line %d: %s.\n", num_line, error);
    exit(1);
}

void printTree(AST_TREE node, int depth) {
    int i;
    if( node != NULL ) {        
        for( i = 0; i < depth; ++i )
            printf("%s", "|  ");
        char * nodeStr = humanReadableNode(node);
        printf("%s\n", nodeStr);
        free(nodeStr);
        AST_TREE operand = node->operands;
        if(operand) {
            printTree(operand, depth+1);
        }

        if( node->next != NULL ) {
            for( i = 0; i < depth; ++i )
                printf("%s", "|  ");
            printf("next\n");
            printTree(node->next, depth);
        }
    } else {
        for( i = 0; i < depth; ++i )
            printf("%s", "|  ");
        printf("NULL\n");
    }
}

extern int yydebug;
extern FILE *yyin;
extern void yyparse();

int main(int argc, const char *argv[]) {
//    yydebug = 1;
    if(argc < 2) {
        fprintf(stderr, "You must give a filename as argument.");
        return EXIT_FAILURE;
    } 
    
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    
    fillSymbols(root);

    printTree(root, 0);

    freeTree(root);
    return EXIT_SUCCESS;
}

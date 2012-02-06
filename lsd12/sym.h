/*
 * sym.h
 *
 *  Created on: Jan 29, 2012
 *      Author: scotchy
 */

#ifndef SYM_H_
#define SYM_H_

#include "ast.h"

#define DIFF_DEPTH      1 << 0
#define DIFF_ID         1 << 1
#define DIFF_FCT        1 << 2
#define DIFF_PARAMS     1 << 3
#define DIFF_FORWARD    1 << 4

typedef struct symbol_list {
    char *id;
    int type;

    int pos;        // relative position
    int depth;      // relative depth

    int isFunction;
    int isForward;
    struct symbol_list *paramList;
    int isParam; // 0 = no, 1 = value passed param, 2 = reference passed param

    struct symbol_list *next;
    
    int num_line; // line where the symbol appears
} *SYMLIST;

/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol );
SYMLIST findVarSymbol(SYMLIST list, char *id);
SYMLIST findParentFunctionSymbol(AST_TREE node);
SYMLIST findFunctionSymbol(SYMLIST list, AST_TREE fctCallNode);
SYMLIST createSymbol(char *id, int type, int num_line);
char *printSymbols(SYMLIST s);
void fillSymbols( AST_TREE root );

#endif /* SYM_H_ */

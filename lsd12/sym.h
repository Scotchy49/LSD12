/*
 * sym.h
 *
 *  Created on: Jan 29, 2012
 *      Author: scotchy
 */

#ifndef SYM_H_
#define SYM_H_

#include "ast.h"

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
} *SYMLIST;

/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol );
char *printSymbols(SYMLIST s);
void fillSymbols( AST_TREE root );

#endif /* SYM_H_ */

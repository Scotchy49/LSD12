/* ast.h
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

#ifndef AST_H
#define AST_H

// do not change values below, or also edit humanReadableNodeType() in ast.c
#define AT_VAR     0
#define AT_NB      1
#define AT_OPADD   2
#define AT_OPSUB   3
#define AT_OPMUL   4
#define AT_OPPRINT 5
#define AT_OPREAD  6
#define AT_OPAFF   7
#define AT_OPMOD   8
#define AT_ILIST   9
#define AT_ROOT   10


struct astnode {
  int type;

  int ival;
  char* sval;

  struct astnode * left;
  struct astnode * right;
};

typedef struct astnode * ASTTREE;
typedef struct astnode   ASTNODE;

extern ASTTREE createNode(int type, int ival, char* sval, ASTTREE left, ASTTREE right);
extern void freeNode(ASTTREE node);
extern void freeTree(ASTTREE tree);

extern void printTree(ASTTREE tree);

#endif

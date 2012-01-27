/* pcode.c
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

#include <stdio.h>

#include <stdlib.h>

#include "pcode.h"

int fillSymbolTable(ASTTREE tree, SYMTABLE s)
{
  if (tree == NULL)
    return 0;

  if (tree->type == AT_VAR)
    {
      if (!alreadyIsSymbol(s, tree->sval))
	addToSymbolTable(s, tree->sval);
    }

  fillSymbolTable(tree->left, s);
  fillSymbolTable(tree->right, s);
    
  return 0;
}

int pcodeGenAddress(ASTTREE tree, SYMTABLE s)
{
  STITEM* node;
  int location;

  if (tree == NULL)
    return 0;

  switch (tree->type) {

  case AT_VAR: // variable
    location = getLocation(s, tree->sval);
    if (location < 0)
      {
	// this should NOT happen, since it will cause havoc on adress space
	fprintf(stderr,"KO\n");
	printf("ERROR : (!!) pcodeGenAddress : VAR '%s' has no location\n", tree->sval);
	exit(1);
      }
    else 
      printf("; loading ADRESS of variable '%s'\n", tree->sval);
      printf("lda i 0 %d\n",location);
    break;

  default:
    fprintf(stderr,"KO\n");
    printf("ERROR : unrecognized type=%d in pcodeGenAddress(..)\n", tree->type);
    exit(1);
  }

  return 0;
}


int pcodeGenValue(ASTTREE tree, SYMTABLE s)
{
  STITEM* node;
  int location;
  static int staticlabel = 0;
  int label = staticlabel;

  staticlabel++;

  if (tree == NULL)
    return 0;

  switch (tree->type) {

  case AT_ROOT:
    printf("; ssp 2 (0 & 1) + memory used for variables (2 temporary locations for modulo algorithm)\n");
    printf("ssp %d\n", (getMaxMemoryUsage(s) <= 0) ? 2 : getMaxMemoryUsage(s) );
    pcodeGenValue(tree->left,s);
    printf("; end of program\n");
    printf("stp\n");
    break;

  case AT_VAR: // variable
    location = getLocation(s, tree->sval);
    if (location < 0)
      {
	// this should NOT happen, since it will cause havoc on address space
	fprintf(stderr,"KO\n");
	printf("ERROR : (!!) pcodeGenValue : VAR '%s' has no location\n", tree->sval);
	exit(1);
      }
    else 
      printf("; loading VALUE of variable '%s'\n", tree->sval);
      printf("lda i 0 %d\nind i\n",location);
    break;

  case AT_NB: // raw number
    printf("; loading constant value '%d'\n", tree->ival);
    printf("ldc i %d\n", tree->ival);
    break;

  case AT_OPADD: // sum
    printf("\n; begin of sum %d\n", label);
    pcodeGenValue(tree->left, s);
    pcodeGenValue(tree->right, s);
    printf("add i\n");
    printf("; end of sum %d\n", label);
    break;

  case AT_OPSUB: // substraction
    printf("\n; begin of substraction %d\n", label); 
    pcodeGenValue(tree->left, s);
    pcodeGenValue(tree->right, s);
    printf("sub i\n");
    printf("; end of substraction %d\n", label);
    break;

  case AT_OPMUL: // multiplication
    printf("\n; begin of multiplication %d\n", label);
    pcodeGenValue(tree->left, s);
    pcodeGenValue(tree->right, s);
    printf("mul i\n");
    printf("; end of the multiplication %d\n", label);
    break;

  case AT_OPPRINT: // print command
    printf("\n; begin of print %d\n", label);
    pcodeGenValue(tree->left, s);
    printf("prin\n");
    printf("; end of print %d\n", label);
    break;

  case AT_OPREAD: // read command
    printf("\n; begin of read %d\n", label);
    pcodeGenAddress(tree->left, s);
    printf("read\nsto i\n");
    printf("; end of read %d\n", label);
    break;

  case AT_OPAFF:
    printf("\n; begin of affectation %d\n", label);
    pcodeGenAddress(tree->left, s);
    pcodeGenValue(tree->right, s);
    printf("sto i\n");
    printf("; end of affectation %d\n", label);
    break;

  case AT_OPMOD:

    /* x mod y  (x,y >= 0)
     *   put y into cell 1 so we can easily remember it
     *   put x into cell 0 so we can easily update it
     *
     * x mod y = 'ERROR' if (y==0)
     *           'x after while !(x < y) x := x-y' else
     */

    printf("\n; begin of modulo %d\n", label);

    printf("\n; saves computed arguments\n");

    /* x */
    printf("; saving x value\n");
    printf("lda i 0 0\n");
    pcodeGenValue(tree->left, s);
    printf("sto i\n");
    printf("; done with saving x value\n\n");

    /* y */
    printf("; saving y value\n");
    printf("lda i 0 1\n");
    pcodeGenValue(tree->right, s);
    printf("sto i\n");
    printf("; done with saving y value\n\n");

    /* checks if y == 0 */
    printf("\n; checks if y == 0\n");
    printf("lda i 0 1\n");
    printf("ind i\n");
    printf("ldc i 0\n");
    printf("equ i\n");
    printf("fjp @mod_not_zero_%d\n", label);
    
    /* here y == 0 */
    printf("; mod division by 0 !!\n");
    printf("stp\n");

    /* else y != 0 */
    printf("\n; begin of mod inner loop\n");
    printf("define @mod_not_zero_%d\n",label);
    
    /* checks if x < y  */
    printf("\n; !(x < y) ?\n");
    printf("lda i 0 0\nind i\n");
    printf("lda i 0 1\nind i\n");
    printf("les i\n");
    printf("not b\n");
    printf("fjp @mod_end_%d\n",label);

    /* !(x<y) -> x := x-y ; goto @mod_not_zero_%d */
    printf("\n; loop body : x := x-y\n");
    printf("lda i 0 0\n");
    printf("lda i 0 0\nind i\n");
    printf("lda i 0 1\nind i\n");
    printf("sub i\n");
    printf("sto i\n");

    printf("ujp @mod_not_zero_%d\n", label);
    
    /* end -> put mod result on top of the stack */
    printf("\n; end, put result on top of the stack\n");
    printf("define @mod_end_%d\n",label);
    printf("lda i 0 0\nind i\n");
    
    printf("\n; end of modulo %d\n", label);

    break;

  case AT_ILIST:
    pcodeGenValue(tree->left,s);
    pcodeGenValue(tree->right,s);
    break;

  default:
    fprintf(stderr,"KO\n");
    printf("ERROR : unrecognized type=%d in pcodeGenValue(..)\n", tree->type);
  }

  return 0;
}

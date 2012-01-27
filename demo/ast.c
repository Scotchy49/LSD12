/* ast.c
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

ASTTREE createNode(int type,
		   int ival,
		   char* sval,
		   ASTTREE left,
		   ASTTREE right)
{
  ASTTREE node = (ASTTREE) malloc(sizeof(ASTNODE));
  if (node == NULL)
    {
      fprintf(stderr,"KO\n");
      printf("ERROR : malloc failed in createNode(..)\n");
      exit(1);
    }
  else
    {
      node->type = type;
      node->ival = ival;
      node->sval = sval;
      node->left = left;
      node->right = right;

      return node;
    }
}

void freeNode(ASTTREE node)
{
  if (node != NULL)
    {
      if (node->sval != NULL) free(node->sval);
      if (node->left != NULL) freeNode(node->left);
      if (node->right != NULL) freeNode(node->right);

      free(node);
    }
}

void freeTree(ASTTREE tree) // idem above but top root is static
{
  if (tree != NULL)
    {
      if (tree->sval != NULL) free(tree->sval);
      if (tree->left != NULL) freeNode(tree->left);
      if (tree->right != NULL) freeNode(tree->right);
    } 
}


char* humanReadableNodeType(int type)
{
  switch(type) {

  case AT_VAR:      return "AT_VAR"; break;
  case AT_NB:       return "AT_NB"; break;
  case AT_OPADD:    return "AT_OPADD"; break;
  case AT_OPSUB:    return "AT_OPSUB"; break;
  case AT_OPMUL :   return "AT_OPMUL"; break;
  case AT_OPPRINT : return "AT_OPPRINT"; break;
  case AT_OPREAD :  return "AT_OPREAD"; break;
  case AT_OPAFF :   return "AT_OPAFF"; break;
  case AT_ILIST :   return "AT_ILIST"; break;
  case AT_OPMOD:    return "AT_OPMOD"; break;
  case AT_ROOT:     return "AT_ROOT"; break;
  default :         return "??";
  }
}

void printTree(ASTTREE tree)
{
  if (tree != NULL) 
    {
      printf("; [%p] type=%s, ival=%d, sval=", tree, humanReadableNodeType(tree->type), tree->ival);
      if (tree->sval == NULL) printf("NULL");
      else printf("'%s'", tree->sval);
      printf(", left=%p, right=%p\n", tree->left, tree->right);

      printTree(tree->left);
      printTree(tree->right);
    }
}

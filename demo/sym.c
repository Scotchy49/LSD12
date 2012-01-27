/* sym.c
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "sym.h"

#define NO_LOC -1

STITEM * createSTNode()
{
  STITEM * node = (STITEM * ) malloc(sizeof(STITEM));

  if (node == NULL)
    {
      fprintf(stderr,"KO\n");
      printf("ERROR : cannot malloc in createSTNode()\n");
      exit(1);
    }

  node->id = NULL;
  node->location = NO_LOC;
  node->next = NULL;

  return node;
}

SYMTABLE createSymbolTable()
{
  SYMTABLE s = (SYMTABLE) createSTNode();
  return s;
}

void freeSymbolTable(SYMTABLE s)
{
  if (s != NULL)
    {
      //if (s->id != NULL) free(s->id);
      freeSymbolTable(s->next);
      free(s);
    }
}

STITEM * symbolLookup(SYMTABLE s, char* name)
{
  if (s == NULL) return NULL;
  else
    if (s->next == NULL) return NULL;
    else
      {
	if (strcmp(s->id, name) == 0) return s;
	else return symbolLookup(s->next, name);
      }
}

int alreadyIsSymbol(SYMTABLE s, char* name)
{
  return (symbolLookup(s,name) == NULL) ? 0 : 1;
}

int addToSymbolTable(SYMTABLE s, char* name)
{
  if (alreadyIsSymbol(s,name)) return 0;
  else 
    {
      while (s->next != NULL) s = s->next;
      
      s->id = name;
      s->next = createSTNode();

      return 1;
    }
}

int computeLocations(SYMTABLE s)
{
  SYMTABLE local = s;
  int available = 2; /* first available mem cell is 2, 
		      * because 0 and 1 are used by modulo algorithm */

  while (local != NULL) {
    if (local->next != NULL)
      {
	local->location = available;
	
	available++;
      }
    local = local->next;
  }
}

int getLocation(SYMTABLE s, char* name)
{
  STITEM * node = symbolLookup(s, name);

  if (node == NULL) return -1;
  else 
    {
      if (node->location == NO_LOC)
	// need to compute locations before using them
	computeLocations(s);

      return node->location;
    }
}

int getMaxMemoryUsage(SYMTABLE s)
{
  SYMTABLE tmp = s;
  int max = 0;

  while (tmp != NULL)
    {
      max = (max < tmp->location+1) ? tmp->location+1 : max;
      tmp = tmp->next;
    }
  return max;
}

void printSymbolTable(SYMTABLE s)
{
  if (s != NULL)
    {
      if (s->next != NULL)
	{
	  printf("; [%p] id=", s);  
	  if (s->id == NULL) printf("NULL");
	  else printf("'%s'", s->id);
	  printf(", location=%d, next=%p\n", s->location, s->next);
	  
	  printSymbolTable(s->next);
	}
    }
}

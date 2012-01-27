/* sym.h
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

#ifndef SYM_H
#define SYM_H

struct _stitem {
  char* id;
  int location;
  
  struct _stitem* next;
};

typedef struct _stitem STITEM;


typedef STITEM * SYMTABLE;


extern SYMTABLE createSymbolTable();
extern void freeSymbolTable(SYMTABLE s);

extern int addToSymbolTable(SYMTABLE s, char* name);
extern int alreadyIsSymbol(SYMTABLE s, char* name);

extern int computeLocations(SYMTABLE s);
extern int getLocation(SYMTABLE s, char* name);
extern int getMaxMemoryUsage(SYMTABLE s);

extern void printSymbolTable(SYMTABLE s);
#endif

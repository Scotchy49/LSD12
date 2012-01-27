/* demo.y
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "sym.h"
#include "pcode.h"

extern int num_lines;
extern char* yytext;

  
// to avoid 'implicit definition'
int yylex(void);
int yyerror(char *str);

ASTTREE root;
%}


// définition du type des variables $x
%union{
        int ival;
        char *sval;
        ASTTREE tval;
}

%token READ PRINT AFFECT
%token LP RP COMMA PLUS MINUS TIMES
%token VAR NB

%left MODULO
%left PLUS MINUS 
%left TIMES


%type <tval> DEMO InstructionList Instruction Expr Var

%%

DEMO : InstructionList { root = createNode(AT_ROOT, 0, NULL, $1, NULL); }
;

InstructionList :                             { $$ = NULL; }
                | InstructionList Instruction { $$ = createNode(AT_ILIST, 0, NULL, $1, $2);}
;

Instruction : PRINT LP Expr RP            {$$ = createNode(AT_OPPRINT, 0, NULL, $3, NULL);}
            | READ  LP Var RP             {$$ = createNode(AT_OPREAD, 0, NULL, $3, NULL);}
            | AFFECT LP Var COMMA Expr RP {$$ = createNode(AT_OPAFF, 0, NULL, $3, $5);}
;

Expr : NB               {$$ = createNode(AT_NB, yylval.ival, NULL, NULL, NULL);}
     | Var              {$$ = $1;}
     | Expr PLUS Expr   {$$ = createNode(AT_OPADD, 0, NULL, $1, $3);} 
     | Expr MINUS Expr  {$$ = createNode(AT_OPSUB, 0, NULL, $1, $3);}
     | Expr TIMES Expr  {$$ = createNode(AT_OPMUL, 0, NULL, $1, $3);}
     | LP Expr RP       {$$ = $2;}
     | Expr MODULO Expr {$$ = createNode(AT_OPMOD, 0, NULL, $1, $3);}
;

Var : VAR { $$ = createNode(AT_VAR, 0, yylval.sval, NULL, NULL);}
;

%%

int yyerror(char *str)
{
        fprintf(stderr,"KO\n");
        printf("ERROR '%s' AT LINE  %d : UNRECOGNISED '%s'\n",
                str,num_lines,yytext);
        exit(0);
}
int main()
{ 
  SYMTABLE sym;

  printf("; *** DEMO compiler\n");
  printf("; *** H. Toussaint (hto@info.fundp.ac.be), 14/06/05\n");
  printf(";\n");

  printf(";*** BEGIN yyparse() ***\n");
  yyparse();
  printf(";*** END yyparse() ***\n");
  
  printf(";*** BEGIN printTree(..) ***\n");
  printTree(root);
  printf(";*** END printTree(..) ***\n");

  printf(";*** BEGIN SymbolTable ***\n");
  sym = createSymbolTable();
  fillSymbolTable(root, sym);
  printf(";*** END SymbolTable ***\n");
  
  printf(";*** BEGIN printSymbolTable(..) ***\n");
  printSymbolTable(sym);
  printf(";*** END printSymbolTable(..) ***\n");

  printf(";*** BEGIN computeLocations(..) ***\n");
  computeLocations(sym);
  printf(";*** END computeLocations(..) ***\n");

  printf(";*** BEGIN printSymbolTable(..) +locations ***\n");
  printSymbolTable(sym);
  printf(";*** END printSymbolTable(..) +locations ***\n");

  printf(";*** BEGIN PCodeGeneration ***\n");
  pcodeGenValue(root, sym);
  printf(";*** END PCodeGeneration ***\n");
  
  printf(";*** BEGIN Cleaning ***\n");
  freeNode(root);
  freeSymbolTable(sym);
  printf(";*** END Cleaning ***\n");

  fprintf(stderr,"OK\n");
  
  return 0;
}

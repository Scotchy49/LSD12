/* demo.l
 *
 * part of the DEMO compiler
 *
 * H. Toussaint (hto@info.fundp.ac.be), 14/06/05
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "y.tab.h"

int num_lines=1;
%}

%option noyywrap

var       [A-Za-z]*
nbr       (0|[1-9][0-9]*)
blank     [ \t]+

%%

"print"      {return PRINT;}
"read"       {return READ;}
"affect"     {return AFFECT;}

"+"          {return PLUS;}
"-"          {return MINUS;}
"*"          {return TIMES;}
"("          {return LP;}
")"          {return RP;}
","          {return COMMA;}
"mod"        {return MODULO;}

{var}        {
                yylval.sval=(char*)calloc(strlen(yytext)+1,sizeof(char));
		strcpy(yylval.sval,yytext);
		return VAR;
             }

{nbr}        {
                yylval.ival=atoi(yytext);
		return NB;
             }

"\n"         {++num_lines;}

{blank}      {/*On passe*/}

.            {
                fprintf(stderr,"KO\n");
                printf("ERROR : invalid '%s' in line %d\n",yytext, num_lines);
                exit(0);
             }

%%

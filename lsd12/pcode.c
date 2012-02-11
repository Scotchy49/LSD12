#include <stdio.h>
#include <stdlib.h>
#include "pcode.h"
#include "ast.h"

int gen = 0;

int getFunctionStackSize( AST_TREE fct ) {
    int i = 0;
    AST_TREE decl = getNodeOperand(fct, OP_FUNCTION_DECLBLOCK)->operands;
    while(decl) {
        if( decl->type == OP_FUNCTION_VAR_DECL ) {
            if( getNodeOperand(decl, OP_VAR_TYPE)->intVal == TYPE_ISET ) {
                i += 2;
            } else {
                i++;
            }
        }
        decl = decl->next;
    }
    
    AST_TREE params = getNodeOperand(fct, OP_FUNCTION_PARAMS)->operands;
    while( params ) {
        if( getNodeOperand(params, OP_VAR_TYPE)->intVal == TYPE_ISET ) {
            i += 2;
        } else {
            i++;
        }
        params = params->next;
    }
    return i;
}

int getNumberOfParams(SYMLIST fct) {
    SYMLIST paramBloc = fct->paramList;
    int i = 0;
    while(paramBloc) {
        ++i;
        if(fct->type == TYPE_ISET)
            ++i;
        paramBloc = paramBloc->next;
    }
    return i;
}

void printISetAlgorithms() {
    // contains
    printf("define @iset_contains\n"
    "ssp 5\n"
    "ldc a 3\n"
    "ldc b 0\n"
    "sto b\n"
    "ldc a 2\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ldc a 4\n"
    "ldc i 0\n"
    "sto i\n"
    "define @containsWhile\n"
    "ldc a 3\n"
    "ind b\n"
    "not b\n"
    "fjp @containsEndWhile\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "les i\n"
    "fjp @containsEndWhile\n"
    "ldc a 4\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc i 1\n"
    "add i\n"
    "sto i\n"
    "ldc a 3\n"
    "ldc a 2\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc a 1\n"
    "ind i\n"
    "equ i\n"
    "sto b\n"
    "ldc a 2\n"
    "ldc a 2\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ujp @containsWhile\n"
    "define @containsEndWhile\n"
    "lda b 0 0\n"
    "ldc a 3\n"
    "ind b\n"
    "sto b\n"
    "retf\n");
    
    //add
    printf("define @iset_add\n"
    "ssp 5\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc i 0\n"
    "equ i\n"
    "fjp @addToSetElseSizeDiffZero\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc i 2\n"
    "new\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ldc a 1\n"
    "ind i\n"
    "sto i\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "ldc a 0\n"
    "sto a\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ldc i 1\n"
    "sto i\n"
    "ujp @addToSetEndIfSizeZero\n"
    "define @addToSetElseSizeDiffZero\n"
    "ldc a 2\n"
    "ldc a 0\n"
    "ind a\n"
    "sto a\n"
    "ldc a 3\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ldc a 4\n"
    "ldc i 0\n"
    "sto i\n"
    "define @addToSetWhileFindWhereToInsert\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "leq i\n"
    "fjp @addToSetEndWhileFindWhereToInsert\n"
    "ldc a 3\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc a 1\n"
    "ind i\n"
    "les i\n"
    "fjp @addToSetEndWhileFindWhereToInsert\n"
    "ldc a 4\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc i 1\n"
    "add i\n"
    "sto i\n"
    "ldc a 2\n"
    "ldc a 3\n"
    "ind a\n"
    "sto a\n"
    "ldc a 3\n"
    "ldc a 3\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ujp @addToSetWhileFindWhereToInsert\n"
    "define @addToSetEndWhileFindWhereToInsert\n"
    "ldc a 2\n"
    "ind a\n"
    "ldc i 2\n"
    "new\n"
    "ldc a 2\n"
    "ind a\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ldc a 1\n"
    "ind i\n"
    "sto i\n"
    "ldc a 2\n"
    "ind a\n"
    "ind a\n"
    "ldc a 3\n"
    "ind a\n"
    "sto a\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc i 1\n"
    "add i\n"
    "sto i\n"
    "define @addToSetEndIfSizeZero\n"
    "define @addToSetEndAddToSet\n"
    "retp\n");
    
    //remove
    printf("define @iset_remove\n"
    "ssp 5\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc i 0\n"
    "equ i\n"
    "fjp @removeFromSetElseNonEmptySet\n"
    "ujp @removeFromSetEndRemove\n"
    "define @removeFromSetElseNonEmptySet\n"
    "ldc a 2\n"
    "ldc a 0\n"
    "ind a\n"
    "sto a\n"
    "ldc a 3\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ldc a 4\n"
    "ldc i 1\n"
    "sto i\n"
    "define @removeFromSetWhileNotVal\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "leq i\n"
    "fjp @removeFromSetEndWhileNotVal\n"
    "ldc a 1\n"
    "ind i\n"
    "ldc a 3\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "neq i\n"
    "fjp @removeFromSetEndWhileNotVal\n"
    "ldc a 4\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc i 1\n"
    "add i\n"
    "sto i\n"
    "ldc a 2\n"
    "ldc a 3\n"
    "ind a\n"
    "sto a\n"
    "ldc a 3\n"
    "ldc a 3\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ujp @removeFromSetWhileNotVal\n"
    "define @removeFromSetEndWhileNotVal\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "leq i\n"
    "fjp @removeFromSetEndIfValEqu\n"
    "ldc a 1\n"
    "ind i\n"
    "ldc a 3\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "equ i\n"
    "fjp @removeFromSetEndIfValEqu\n"
    "ldc a 2\n"
    "ind a\n"
    "ldc a 3\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "ldc i 1\n"
    "sub i\n"
    "sto i\n"
    "define @removeFromSetEndIfValEqu\n"
    "define @removeFromSetEndRemove\n"
    "retp\n");
    
    //max
    printf("define @iset_max\n"
    "ssp 5\n"
    "ldc a 2\n"
    "ldc a 0\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ldc a 4\n"
    "ldc i 1\n"
    "sto i\n"
    "define @maxInSetWhileNotEnd\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc a 0\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "les i\n"
    "fjp @maxInSetEndWhileNotEnd\n"
    "ldc a 4\n"
    "ldc a 4\n"
    "ind i\n"
    "ldc i 1\n"
    "add i\n"
    "sto i\n"
    "ldc a 2\n"
    "ldc a 2\n"
    "ind a\n"
    "ind a\n"
    "sto a\n"
    "ujp @maxInSetWhileNotEnd\n"
    "define @maxInSetEndWhileNotEnd\n"
    "lda i 0 0\n"
    "ldc a 2\n"
    "ind a\n"
    "ldc a 1\n"
    "add a\n"
    "ind i\n"
    "sto i\n"
    "retf\n");
    
    printf("define @iset_min\n"
        "ssp 5\n"
        "lda i 0 0\n"
        "ldc a 0\n"
        "ind a\n"
        "ldc a 1\n"
        "add a\n"
        "ind i\n"
        "sto i\n"
        "retf\n");
}

void generateAdressPCode( AST_TREE node ) {
    if( node->type == OP_LEXPR ) {
        SYMLIST s = findVarSymbol(node->symbols, node->strVal);
        int paramSize = getNumberOfParams(findParentFunctionSymbol(node));
        
        if( s->isParam == 2 ) {
            printf("lod a ");
        } else {
            printf("lda %s ", getVarTypeName(s->type));
        }
        printf("%d %d\n",  
            findParentFunctionSymbol(node)->depth+1-s->depth, 
            s->pos - (s->type == TYPE_ISET ? 1 : 0)
          );
        return;
    }
    
    // if it is not a variable expr, we just generate the regular PCode for it
    generatePCode(node);
}

void initializeISets(AST_TREE function) {
    AST_TREE decl = getNodeOperand(function, OP_FUNCTION_DECLBLOCK)->operands;
    int paramSize = getNumberOfParams(function->symbols);
    while( decl ) {
        if( decl->type == OP_FUNCTION_VAR_DECL && getNodeOperand(decl, OP_VAR_TYPE)->intVal == TYPE_ISET ){
            // on met la taille à 0 pour cet iset
            printf("lda i 0 %d\n", decl->symbols->pos);
            printf("ldc i 0\n");
            printf("sto i\n");
            
            // on alloue de l'espace pour cet iset sur le heap et on fait pointer
            printf("lda a 0 %d\n", decl->symbols->pos - 1);
            printf("ldc i 1\n");
            printf("new\n");
            
            // on termine la liste avec l'adresse 0 du stack
            printf("lda a 0 %d\n", decl->symbols->pos - 1);
            printf("ind a\n");
            printf("ldc a 0\n");
            printf("sto a\n");
        }
        decl = decl->next;
    }
}

void generatePCode(AST_TREE node) {
    if( node != NULL ) {
        printf(";%d: %s\n", node->num_line, humanReadableNode(node));

        if( node->type == OP_PROGRAM ) {
            AST_TREE mainFunc = getNodeOperand(node, OP_FUNCTION);
            int s = getFunctionStackSize(mainFunc);
            printf("ssp %d\n", 5+s);
            initializeISets(mainFunc);
            printf("ujp @begin\n");
            printISetAlgorithms();
            generatePCode(getNodeOperand(mainFunc, OP_FUNCTION_DECLBLOCK));
            printf("define @begin\n");
            generatePCode(getNodeOperand(getNodeOperand(mainFunc, OP_FUNCTION_BODY), OP_INSTRUCTION_LIST));
            printf("stp\n");
        } else if( node->type == OP_FUNCTION_FORWARD ) {
            // when we forward declarate, we need to assign an ID to the function NOW !
            SYMLIST symbol = node->symbols;
            symbol->uniqueId = ++gen;
            symbol->ref->uniqueId = symbol->uniqueId;
        } else if( node->type == OP_FUNCTION ) {
            int s = getFunctionStackSize(node);
            char *id = getNodeOperand(node, OP_ID)->strVal;
            SYMLIST symbol = node->symbols;
            
            if( symbol->uniqueId == -1 )
                symbol->uniqueId = ++gen;
            
            printf("define @%s_%d\n", id, symbol->uniqueId);
            printf("ssp %d\n", 5+s);
            initializeISets(node);
            printf("ujp @%s_%dBody\n", id, symbol->uniqueId);
            generatePCode(getNodeOperand(node, OP_FUNCTION_DECLBLOCK));
            printf("define @%s_%dBody\n", id, symbol->uniqueId);
            generatePCode(getNodeOperand(getNodeOperand(node, OP_FUNCTION_BODY), OP_INSTRUCTION_LIST));
            
            if(getNodeOperand(node, OP_FUNCTION_TYPE)->intVal == TYPE_VOID ) {
                printf("retp\n");
            } else {
                printf("retf\n");
            }
        } else if( node->type == OP_RETURN ) {
            int type = findParentFunctionSymbol(node)->type;
            printf("lda %s 0 0\n", getVarTypeName(type));
            generatePCode(node->operands);
            printf("sto %s\n", getVarTypeName(type));
            printf("retf\n");
        } else if( node->type == OP_FUNCTION_CALL ) {
            printf("mst 0\n");
            AST_TREE params = getNodeOperand(node, OP_FUNCTION_CALL_PARAMS)->operands;
            SYMLIST fct = findFunctionSymbol(node->symbols, node);
            SYMLIST paramList = fct->paramList;
            int i;
            for( i = 0; params; ++i ) {
                if( paramList->isParam == 2 ) {
                    generateAdressPCode(params);
                } else {
                    generatePCode(params);
                }
                params = params->next;
                paramList = paramList->next;
            }
            printf("cup %d @%s_%d\n", i, fct->id, fct->uniqueId);
        } else if(node->type == OP_FUNCTION_DECLBLOCK) {
            AST_TREE declList = node->operands;
            while(declList) {
                if( declList->type == OP_FUNCTION || declList->type == OP_FUNCTION_FORWARD ) {
                    generatePCode(declList);
                }
                declList = declList->next;
            }
        } else if( node->type == OP_INSTRUCTION_LIST ) {
            AST_TREE instructions = node->operands;
            while(instructions) {
                generatePCode(instructions);
                instructions = instructions->next;
            }
        } else if( node->type == OP_LEXPR ) {
            SYMLIST s = findVarSymbol(node->symbols, node->strVal);
            generateAdressPCode(node);
            printf("ind %s\n", getVarTypeName(s->type));
        } else if( node->type == OP_CONSTANT_INT ) {
            printf("ldc i %d\n", node->intVal);
        } else if( node->type == OP_CONSTANT_BOOL ) {
            printf("ldc b %d\n", node->intVal);
        } else if( node->type == OP_OR ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            int g = ++gen;
            generatePCode(left);
            printf("not b\n");
            printf("fjp @true_%d\n", g);
            generatePCode(right);
            printf("ujp @end_%d\n", g);
            printf("define @true_%d\n", g);
            printf("ldc b 1\n");
            printf("define @end_%d\n", g);
        } else if( node->type == OP_AND ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            int g = ++gen;
            generatePCode(left);
            printf("not b\n");
            printf("fjp @true_%d\n", g);
            printf("ujp @false_%d\n", g);
            printf("define @true_%d\n", g);
            generatePCode(right);
            printf("ujp @end_%d\n", g);
            printf("define @false_%d\n", g);
            printf("ldc b 0\n");
            printf("define @end_%d\n", g);
        } else if( node->type == OP_NOT ) {
            AST_TREE operand = node->operands;
            generatePCode(operand);
            printf("not b\n");
        } else if( node->type == OP_EQ ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("equ i\n");
        } else if( node->type == OP_LT ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("les i\n");
        } else if( node->type == OP_LTE ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("leq i\n");
        } else if( node->type == OP_ASSIGN ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generateAdressPCode(left);
            generatePCode(right);
            SYMLIST s = findVarSymbol(left->symbols, left->strVal);
            printf("sto %s\n", getVarTypeName(s->type));
        } else if( node->type == OP_PLUS ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("add i\n");
        } else if( node->type == OP_MINUS ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("sub i\n");
        } else if( node->type == OP_TIMES ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("mul i\n");
        } else if( node->type == OP_DIV ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("div i\n");
        } else if( node->type == OP_WRITE ) {
            generatePCode(node->operands);
            printf("prin\n");
        } else if( node->type == OP_READ ) {
            generateAdressPCode(node->operands);
            printf("read\n");
            printf("sto i\n");
        } else if( node->type == OP_IF ) {
            AST_TREE cond = node->operands;
            AST_TREE then = cond->next;
            int g = ++gen;
            
            generatePCode(cond);
            printf("fjp @fi_%d\n", g);
            generatePCode(then);
            printf("define @fi_%d\n", g);
        } else if( node->type == OP_IF_ELSE ) {
            AST_TREE cond = node->operands;
            AST_TREE then = cond->next;
            AST_TREE els  = then->next;
            int g = ++gen;
            
            generatePCode(cond);
            printf("fjp @else_%d\n", g);
            generatePCode(then);
            printf("ujp @fi_%d\n", g);
            printf("define @else_%d\n", g);
            generatePCode(els);
            printf("define @fi_%d\n", g);
        } else if( node->type == OP_WHILE ) {
            AST_TREE cond = node->operands;
            AST_TREE then = cond->next;
            int g = ++gen;
            
            printf("define @while_%d\n", g);
            generatePCode(cond);
            printf("fjp @od_%d\n", g);
            generatePCode(then);
            printf("ujp @while_%d\n", g);
            printf("define @od_%d\n", g);
        } else if( node->type == OP_ADD_ISET ) {
            printf("ldc a 1\n");
            generatePCode(node->operands);
            printf("sto i\n");
            
            SYMLIST iset = findVarSymbol(node->symbols, node->operands->next->strVal);
            printf("ldc a 0\n");
            generateAdressPCode(node->operands->next);
            printf("sto a\n");
            
            printf("mst 0\n");
            printf("cup 0 @iset_add\n");
        } else if(node->type == OP_REMOVE_ISET ) {
            printf("ldc a 1\n");
            generatePCode(node->operands);
            printf("sto i\n");
            
            SYMLIST iset = findVarSymbol(node->symbols, node->operands->next->strVal);
            printf("ldc a 0\n");
            generateAdressPCode(node->operands->next);
            printf("sto a\n");
            
            printf("mst 0\n");
            printf("cup 0 @iset_remove\n");
        } else if( node->type == OP_IN ) {
            printf("ldc a 1\n");
            generatePCode(node->operands);
            printf("sto i\n");
            
            SYMLIST iset = findVarSymbol(node->symbols, node->operands->next->strVal);
            printf("ldc a 0\n");
            generateAdressPCode(node->operands->next);
            printf("sto a\n");
            
            printf("mst 0\n");
            printf("cup 0 @iset_contains\n");
        } else if( node->type == OP_MAX_ISET ) {
            printf("ldc a 0\n");
            generatePCode(node->operands);
            printf("sto a\n");
            
            printf("mst 0\n");
            printf("cup 0 @iset_max\n");
        } else if( node->type == OP_MIN_ISET ) {
            printf("ldc a 0\n");
            generatePCode(node->operands);
            printf("sto a\n");
            
            printf("mst 0\n");
            printf("cup 0 @iset_min\n");
        } else if( node->type == OP_SIZE_ISET ) {
            generateAdressPCode(node->operands);
            printf("ldc a 1\n");
            printf("add a\n");
            printf("ind i\n");
        }
            
    }
}
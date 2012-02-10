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
            i++;
        }
        decl = decl->next;
    }
    
    AST_TREE params = getNodeOperand(fct, OP_FUNCTION_PARAMS)->operands;
    while( params ) {
        i++;
        params = params->next;
    }
    return i;
}

void generateAdressPCode( AST_TREE node ) {
    if( node->type == OP_LEXPR ) {
        SYMLIST s = findVarSymbol(node->symbols, node->strVal);
        if( s->isParam == 2 ) {
            printf("lod a %d %d\n", findParentFunctionSymbol(node)->depth+1-s->depth, s->pos + (s->isParam ? 5 : 0));
        } else {
            printf("lda %s %d %d\n", getVarTypeName(s->type), findParentFunctionSymbol(node)->depth+1-s->depth, s->pos + (s->isParam ? 5 : 0));
        }
        return;
    }
    
    // if it is not a variable expr, we just generate the regular PCode for it
    generatePCode(node);
}

void generatePCode(AST_TREE node) {
    if( node != NULL ) {
        printf(";Generating pcode for node %s\n", humanReadableNode(node));

        if( node->type == OP_PROGRAM ) {
            AST_TREE mainFunc = getNodeOperand(node, OP_FUNCTION);
            int s = getFunctionStackSize(mainFunc);
            printf("ssp %d\n", s);        
            printf("ujp @begin\n");
            generatePCode(getNodeOperand(mainFunc, OP_FUNCTION_DECLBLOCK));
            printf("define @begin\n");
            generatePCode(getNodeOperand(getNodeOperand(mainFunc, OP_FUNCTION_BODY), OP_INSTRUCTION_LIST));
            printf("stp\n");
        }
        
        if( node->type == OP_FUNCTION_FORWARD ) {
            // when we forward declarate, we need to assign an ID to the function NOW !
            SYMLIST symbol = node->symbols;
            symbol->uniqueId = ++gen;
            symbol->ref->uniqueId = symbol->uniqueId;
        }

        if( node->type == OP_FUNCTION ) {
            int s = getFunctionStackSize(node);
            char *id = getNodeOperand(node, OP_ID)->strVal;
            SYMLIST symbol = node->symbols;
            
            if( symbol->uniqueId == -1 )
                symbol->uniqueId = ++gen;
            
            printf("define @%s_%d\n", id, symbol->uniqueId);
            printf("ssp %d\n", 5+s);
            printf("ujp @%s_%dBody\n", id, symbol->uniqueId);
            generatePCode(getNodeOperand(node, OP_FUNCTION_DECLBLOCK));
            printf("define @%s_%dBody\n", id, symbol->uniqueId);
            generatePCode(getNodeOperand(getNodeOperand(node, OP_FUNCTION_BODY), OP_INSTRUCTION_LIST));
            
            if(getNodeOperand(node, OP_FUNCTION_TYPE)->intVal == TYPE_VOID ) {
                printf("retp\n");
            } else {
                printf("retf\n");
            }
        }
        
        if( node->type == OP_RETURN ) {
            int type = findParentFunctionSymbol(node)->type;
            printf("lda %s 0 0\n", getVarTypeName(type));
            generatePCode(node->operands);
            printf("sto %s\n", getVarTypeName(type));
            printf("retf\n");
        }
        
        if( node->type == OP_FUNCTION_CALL ) {
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
        }
        
        if(node->type == OP_FUNCTION_DECLBLOCK) {
            AST_TREE declList = node->operands;
            while(declList) {
                if( declList->type == OP_FUNCTION || declList->type == OP_FUNCTION_FORWARD ) {
                    generatePCode(declList);
                }
                declList = declList->next;
            }
        }

        if( node->type == OP_INSTRUCTION_LIST ) {
            AST_TREE instructions = node->operands;
            while(instructions) {
                generatePCode(instructions);
                instructions = instructions->next;
            }
        }
        
        if( node->type == OP_LEXPR ) {
            SYMLIST s = findVarSymbol(node->symbols, node->strVal);
            generateAdressPCode(node);
            printf("ind %s\n", getVarTypeName(s->type));
        }
        
        if( node->type == OP_CONSTANT_INT ) {
            printf("ldc i %d\n", node->intVal);
        }
        
        if( node->type == OP_CONSTANT_BOOL ) {
            printf("ldc b %d\n", node->intVal);
        }
        
        if( node->type == OP_OR ) {
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
        }
        
        if( node->type == OP_AND ) {
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
        }
        
        if( node->type == OP_NOT ) {
            AST_TREE operand = node->operands;
            generatePCode(operand);
            printf("not b\n");
        }
        
        if( node->type == OP_EQ ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("equ i\n");
        }
        
        if( node->type == OP_LT ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("les i\n");
        }
        
        if( node->type == OP_LTE ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("leq i\n");
        }
        
        if( node->type == OP_ASSIGN ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generateAdressPCode(left);
            generatePCode(right);
            SYMLIST s = findVarSymbol(left->symbols, left->strVal);
            printf("sto %s\n", getVarTypeName(s->type));
        }
        
        if( node->type == OP_PLUS ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("add i\n");
        }
        
        if( node->type == OP_MINUS ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("sub i\n");
        }
        
        if( node->type == OP_TIMES ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("mul i\n");
        }
        
        if( node->type == OP_DIV ) {
            AST_TREE left = node->operands;
            AST_TREE right = left->next;
            generatePCode(left);
            generatePCode(right);
            printf("div i\n");
        }
        
        if( node->type == OP_WRITE ) {
            generatePCode(node->operands);
            printf("prin\n");
        }
        
        if( node->type == OP_READ ) {
            generateAdressPCode(node->operands);
            printf("read\n");
            printf("sto i\n");
        }
        
        if( node->type == OP_IF ) {
            AST_TREE cond = node->operands;
            AST_TREE then = cond->next;
            int g = ++gen;
            
            generatePCode(cond);
            printf("fjp @fi_%d\n", g);
            generatePCode(then);
            printf("define @fi_%d\n", g);
        }
        
        if( node->type == OP_IF_ELSE ) {
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
        }
        
        if( node->type == OP_WHILE ) {
            AST_TREE cond = node->operands;
            AST_TREE then = cond->next;
            int g = ++gen;
            
            printf("define @while_%d\n", g);
            generatePCode(cond);
            printf("fjp @od_%d\n", g);
            generatePCode(then);
            printf("ujp @while_%d\n", g);
            printf("define @od_%d\n", g);
        }
            
    }
}
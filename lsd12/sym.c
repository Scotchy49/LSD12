
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "sym.h"
#include "type.h"

char *printSymbol(SYMLIST s) {
    char *symbol = malloc(255);
    char *fct = "";
    if( s->isFunction ) {
        fct = malloc(127);
        strcpy(fct, "(");
        SYMLIST params = s->paramList;
        while(params) {
            sprintf(fct, "%s%s%s%s", fct, params->isParam == 2 ? "&" : "", getVarTypeName(params->type), params->next?",":"");
            params = params->next;
        }
        strcat(fct, ")");
    }

    sprintf(symbol, "%s%s%s:%s(%d,%d)", s->isForward ? "-":s->isParam ? "+" : "", s->id, fct, getVarTypeName(s->type), s->depth, s->pos);
    return symbol;
}

SYMLIST appendSymbol( SYMLIST list, SYMLIST symbol ) {
    SYMLIST tmp = list;
    if(tmp) {
        while(tmp->next)
            tmp = tmp->next;
            
        tmp->next = symbol;
        return list;
    }

    return symbol;
}


int symbolcmp(SYMLIST a, SYMLIST b) {
    int result = 0;
    
    // they have to be on the same depth
    // otherwise is it a shadow
    if( a->depth != b->depth ) {
        result |= DIFF_DEPTH ;
    }

    // ID check
    if( strcmp(a->id, b->id) != 0 )
        result |= DIFF_ID;
    
    if( a->isFunction != b->isFunction )
        result |= DIFF_FCT;

    // for functions, we check if the length and type of the operators are the same
    if( a->isFunction && b->isFunction ) {
        SYMLIST p_a = a->paramList;
        SYMLIST p_b = b->paramList;
        while(p_a || p_b) {
            if( !p_a || !p_b ) {
                result |= DIFF_PARAMS;
                break;
            } else {
                // not the same type
                if( p_a->type != p_b->type ) {
                    result |= DIFF_PARAMS;
                    break;
                }

                p_a = p_a->next;
                p_b = p_b->next;
            }
        }

        if( a->isForward ^ b->isForward )
            result |= DIFF_FORWARD;
    }

    return result;
}

SYMLIST findSymbol(SYMLIST list, SYMLIST symbol) {
    while(list) {
        int diff = symbolcmp(list, symbol);
        if( diff & DIFF_ID || diff & DIFF_FCT || diff & DIFF_PARAMS ) {
                list = list->next;
        } else {
                return list;
        }
    }
    return NULL;
}

SYMLIST findVarSymbol(SYMLIST list, char*id) {
    SYMLIST symbol = createSymbol(id,-1,-1);
    return findSymbol(list, symbol);
}


SYMLIST findParentFunctionSymbol(AST_TREE node) {
    while(node) {
        if( node->parent->type == OP_FUNCTION ) {
            return node->parent->symbols;
        }
        node = node->parent;
    }
    return NULL;
}

SYMLIST findFunctionSymbol(SYMLIST list, AST_TREE functionCall) {
    char *id = getNodeOperand(functionCall, OP_ID)->strVal;

    SYMLIST s = createSymbol(id, -1, -1);
    s->isFunction = 1;
    s->depth = -1;

    AST_TREE param = getNodeOperand(functionCall, OP_FUNCTION_CALL_PARAMS)->operands;

    while(param) {
        SYMLIST ps = createSymbol(NULL, getType(param), -1);
        ps->isParam = 1;
        s->paramList = appendSymbol(s->paramList, ps);
        param = param->next;
    }
    
    while(list) {
        int diff = symbolcmp(list, s);
        if( diff == DIFF_DEPTH || (diff == (DIFF_DEPTH | DIFF_FORWARD)) ) {
            return list;
        } 
        list = list->next;
    }
    return NULL;
}

void checkNameConstraints(SYMLIST list, SYMLIST symbol) {
    SYMLIST tmp = list;
    // check name constraints
    while( tmp && tmp->depth == symbol->depth ) {
        if(symbolcmp(tmp, symbol) == 0) {
            fprintf(stderr, "KO\n");
            fprintf(stderr, "Line %d: %s already defined as %s (line %d)\n", symbol->num_line, printSymbol(symbol), printSymbol(tmp), tmp->num_line);
            exit(EXIT_FAILURE);
        }
        tmp = tmp->next;
    }
}
/*
 * pre: symbol is initialized
 * post: puts symbol at the start of the list
 */
SYMLIST prependSymbol( SYMLIST list, SYMLIST symbol ) {
    if( symbol ) {
        checkNameConstraints(list, symbol);
        symbol->next = list;
        return symbol;
    }
    return list;
}

SYMLIST createSymbol(char *id, int type, int num_line) {
    SYMLIST s = malloc(sizeof *s);
    s->id = id;
    s->type = type;
    s->num_line = num_line;
    s->pos = 0;
    s->depth = 0;
    s->next = NULL;
    s->isFunction = 0;
    s->isForward = 0;
    s->isParam = 0;
    s->paramList = NULL;
    return s;
}

SYMLIST getVarSymbol(AST_TREE node) {

    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_VAR_TYPE)->intVal;

    return createSymbol(id, type, node->num_line);
}



SYMLIST getFunctionSymbol(AST_TREE node) {
    char *id = getNodeOperand(node, OP_ID)->strVal;
    int type = getNodeOperand(node, OP_FUNCTION_TYPE)->intVal;

    SYMLIST s = createSymbol(id, type, node->num_line);
    s->isFunction = 1;
    s->isForward = node->type == OP_FUNCTION_FORWARD;

    AST_TREE param = getNodeOperand(node, OP_FUNCTION_PARAMS)->operands;
    while(param) {
        SYMLIST ps = getVarSymbol(param);
        ps->isParam = 1 + getNodeOperand(param, OP_FUNCTION_PARAM_VAR)->intVal;
        s->paramList = appendSymbol(s->paramList, ps);
        param = param->next;
    }
    return s;
}

SYMLIST getSymbol(AST_TREE node, int depth, int pos) {
    int i;
    SYMLIST sym = NULL;

    if(node->type == OP_FUNCTION || node->type == OP_FUNCTION_FORWARD ) {
        sym = getFunctionSymbol(node);
    } else if (node->type == OP_FUNCTION_VAR_DECL) {
        sym = getVarSymbol(node);
    } else if(node->type == OP_FUNCTION_PARAM ) {
        sym = getVarSymbol(node);
        sym->isParam = 1 + getNodeOperand(node, OP_FUNCTION_PARAM_VAR)->intVal;
    }

    if( sym ) {
        sym->depth = depth;
        sym->pos = pos;
    }

    return sym;
}




char *printSymbols(SYMLIST s) {
    char *symbols = malloc(1024);
    while(s) {
        sprintf(symbols, "%s -> %s", symbols, printSymbol(s));
        s = s->next;
    }
    return symbols;
}


void popSymbols(AST_TREE root) {
    if( root->type == OP_FUNCTION_PARAMS ) {
        AST_TREE paramList = root->operands;
        if( paramList ) {
            while(paramList->next) {
                paramList = paramList->next;
            }
            root->symbols = paramList->symbols;
        }
    } else if( root->type == OP_FUNCTION_DECLBLOCK ) {
        AST_TREE declList = root->operands;
        if( declList ) {
            while(declList->next) {
                declList = declList->next;
            }
            root->symbols = declList->symbols;
        }
    }
}

void populateSymbols( AST_TREE root, SYMLIST inherited, int depth ) {
    int i;
    int pos = 0;
    while( root ) {
        // each node inherits the symbols of its father, so we prepend the current node's
        // symbol with the "already" accessible ones
        SYMLIST nSymbol = getSymbol(root, depth, pos);
        if( nSymbol )
            ++pos;
        
        root->symbols = prependSymbol(inherited, nSymbol);

        // the operands inherit from the operator's symbols, so we will pass those to them
        // while populating them. As we give symbols to the operands, we will discover new
        // symbols. The successors will inherit those.

        // depth is relative to function, so we increase the depth only if we traversed a function operator
        int d = depth;
        if( root->type == OP_FUNCTION || root->type == OP_FUNCTION_FORWARD ) {
            d++;
        }

        populateSymbols(root->operands, root->symbols, d);

        // once we populated the operands, sometimes we need to bubble back new identifiers
        // back to the operator.
        popSymbols(root);
        
        validateType(root);
        
        // the next vertical operators inherit automatically from this node, too
        inherited = root->symbols;
        root = root->next;
    }
}

void fillSymbols( AST_TREE program ) {

    // on créer les symboles
    populateSymbols(program, NULL, -1);

}

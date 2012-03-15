#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "type.h"

extern void printTree(AST_TREE node, int depth);

void error(int line, const char * msg, ...) {
    fprintf(stderr, "KO\n");
    fprintf(stderr, "Line %d: type error:", line, msg);
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

int getType( AST_TREE node ) {
    // int expressions
    if(node->type == OP_CONSTANT_INT) {
        return TYPE_INT;
    }
    
    if( node->type == OP_PLUS || node->type == OP_MINUS || node->type == OP_TIMES || node->type == OP_DIV ) {
        int leftType = getType(node->operands);
        int rightType = getType(node->operands->next);
        if( leftType != TYPE_INT || rightType != TYPE_INT ) {
            error(node->num_line, "Incorrect type of int expression");
        }
        return TYPE_INT;
    }
    
    if(node->type == OP_EQ || node->type == OP_LT || node->type == OP_LTE ) {
        if( getType(node->operands) != TYPE_INT || getType(node->operands->next) != TYPE_INT ) {
            error(node->num_line, "Incorrect type of int expression");
        }
        return TYPE_BOOLEAN;
    }
    
    // bool expressions
    if( node->type == OP_CONSTANT_BOOL ) {
        return TYPE_BOOLEAN;
    }
    
    if( node->type == OP_AND || node->type == OP_OR ) {
        int leftType = getType(node->operands);
        int rightType = getType(node->operands->next);
        if( leftType != TYPE_BOOLEAN || rightType != TYPE_BOOLEAN ) {
            error(node->num_line, "Incorrect type of boolean expression");
        }
        return TYPE_BOOLEAN;
    }
    
    if(node->type == OP_NOT ) {
        if( getType(node->operands) != TYPE_BOOLEAN ) {
            error(node->num_line, "Incorrect type of boolean expression");
        }
        return TYPE_BOOLEAN;
    }
    
    //iset expressions
    if( node->type == OP_ADD_ISET || node->type == OP_REMOVE_ISET ) {
        if( getType(node->operands) != TYPE_INT ) {
            error(node->num_line, "add/remove to/from iset only accepts int params");
        }
        
        //TODO
    }
    
    if( node->type == OP_MIN_ISET || node->type == OP_MAX_ISET || node->type == OP_SIZE_ISET ) {
        if( getType(node->operands) != TYPE_ISET ) {
            error(node->num_line, "Was expecting iset");
        }
        return TYPE_INT;
    }
    
    if( node->type == OP_IN ) {
        if(getType(node->operands) != TYPE_INT || getType(node->operands->next) != TYPE_ISET ) {
            error(node->num_line, "ill-formed 'in' expression");
        }
        return TYPE_BOOLEAN;
    }
    
    // symbol table lookups
    if( node->type == OP_LEXPR ) {
        char *id = node->strVal;
        SYMLIST symbol;
        if( !(symbol = findVarSymbol(node->symbols, id) ) ) {
            error(node->num_line, "unknown variable");
        } 
        return symbol->type;
    }
    
    // function & function calls
    if(node->type == OP_FUNCTION || node->type == OP_FUNCTION_FORWARD ) {
        if( node->type == OP_FUNCTION_FORWARD ) {
            // check if we have a ref to the impl
            if( !node->symbols->ref ) {
                error(node->num_line, "forward declaration not implemented.");
            } else if( node->symbols->type != node->symbols->ref->type ) {
                // the return type of the forward decl must be the same as the return type of the implementation
                error(node->num_line, "forward declaration return type do not match (declared %s, actual is %s)", 
                        getVarTypeName(node->symbols->type), getVarTypeName(node->symbols->ref->type));
            }
        }
        
        // on vérifie que tous les iset sont bien passés par référence
        SYMLIST params = node->symbols->paramList;
        while( params ) {
            if( params->type == TYPE_ISET && params->isParam != 2 ) {
                error(node->num_line, "isets can only be passed by reference");
            }
            params = params->next;
        }
        return getNodeOperand(node, OP_FUNCTION_TYPE)->intVal;
    }
    
    if(node->type == OP_FUNCTION_CALL) {
        SYMLIST fct = findFunctionSymbol(node->symbols, node);
        if( fct == NULL ) {
            error(node->num_line, "function not found %s", getNodeOperand(node, OP_ID)->strVal);
        } 
        
        // vérification de passage des paramètres VAR
        AST_TREE actualParams = getNodeOperand(node, OP_FUNCTION_CALL_PARAMS)->operands;
        SYMLIST params = fct->paramList;
        while( params ) {
            if( params->isParam == 2 && actualParams->type != OP_LEXPR) {
                // si le paramètre formel est de type VAR, alors il faut que le paramètre passé
                // soit de type LExpr ! (mail du 11/02/2012 par Xde)
                error(node->num_line, "expected a LExpr for VAR param");
            }
            params = params->next;
            actualParams = actualParams->next;
        }
        return fct->type;
    }
    
    // misc type checks
    if( node->type == OP_ASSIGN ) {
        int leftType = getType(node->operands);
        if( leftType != TYPE_INT && leftType != TYPE_BOOLEAN ) {
            error(node->num_line, "assignation: can only be applied on ints and bools");
        } else if( leftType != getType(node->operands->next) ) {
            error(node->num_line, "assignation: types do not match.");
        }
    }
    
    if(node->type == OP_READ || node->type == OP_WRITE) {
        int type = getType(node->operands);
        if( type != TYPE_INT ) {
            error(node->num_line, "read/write operations can only be applied to int");
        }
    }
    
    if(node->type == OP_PROGRAM ) {
        AST_TREE mainFunction = getNodeOperand(node, OP_FUNCTION);
        
        if( getType(mainFunction) != TYPE_VOID ) {
            error(mainFunction->num_line, "the main function must be of type void.");
        } 
        
        // check param count
        if( getNodeOperand(mainFunction, OP_FUNCTION_PARAMS)->operands != NULL ) {
            error(mainFunction->num_line, "the main function cannot take any parameters");
        }
    }
    
    if(node->type == OP_IF || node->type == OP_IF_ELSE || node->type == OP_WHILE) {
        // the condition is the first operand
        if(getType(node->operands) != TYPE_BOOLEAN ) {
            error(node->num_line, "the condition must be a boolean");
        }
    }
    
    if(node->type == OP_RETURN ) {
        int functionType = findParentFunctionSymbol(node)->type;
        if( functionType == TYPE_VOID ) {
            error(node->num_line, "void functions cannot return a value.");
        }
        
        if( functionType != getType(node->operands) ) {
            error(node->num_line, "return value not the same as function type");
        }
    }
    
    if( node->type == OP_NEG ) {
        if(getType(node->operands) != TYPE_INT ) {
            error(node->num_line, "negation can only be applied to integers");
        }
        return TYPE_INT;
    }
    
    return -1;
}

void validateType(AST_TREE node) {
    // a node is valid if all its tree is correctly typed
    while( node ) {
        getType(node);
        validateType(node->operands);
        node = node->next;
    }
}


//#include <stdio.h>
//#include <stdlib.h>
//#include "sym.h"

//SYMBOL createSymbolList() {
//    SYMBOL s = NULL;
//    return s;
//}

//SYMBOL addSymbol( char *symbol, int type, int depth, SYMBOL prev ) {
//    SYMBOL s = malloc(sizeof *s);
//    s->id = symbol;
//    s->type = type;
//    s->depth = depth;
//    s->prev = prev;
//    return s;
//}

//SYMBOL pushSymbols( SYMBOL prev, SYMBOL next ) {
//    next->prev = prev;
//    return next;
//}

//AST_TREE getChild( int type, AST_TREE root ) {
//    int i;
//    for( i = 0; i < root->op_count; ++i ) {
//        if( root->operands[i]->type == type )
//            return root->operands[i];
//    }
//}

//SYMBOL getSymbol( AST_TREE root, int depth, SYMBOL s ) {
//    /*
//    if( root->type == OP_FUNCTION ) {
//        char *id = getChild(OP_ID, root)->strVal;
//        VAR_TYPE type = getChild(OP_FUNCTION_TYPE, root)->intVal;
//        // todo handle parameters
//        s = addSymbol(id, type, depth, s);
//        AST_TREE params = getChild(OP_FUNCTION_PARAMS, root);
//        SYMBOL paramSymbols = getSymbol( params, depth, s );
//        if( paramSymbols != NULL )
//            s = pushSymbols( s, paramSymbols );

//        AST_TREE childs = getChild(OP_FUNCTION_DECLBLOCK, root);
//        int i;
//        for( i = 0; i < childs->op_count; ++i ) {
//            getSymbol( childs->operands[i], depth + 1, s );
//        }
//    } else if( root->type == OP_FUNCTION_PARAMS ) {
//        int i;
//        for( i = 0; i < root->op_count; ++i ) {
//            AST_TREE param = root->operands[i];
//            s = pushSymbols(s, getSymbol(param, depth, s));
//        }
//    } else if( root->type == OP_FUNCTION_PARAM || root->type == OP_FUNCTION_VAR_DECL ) {
//        char *id = getChild(OP_ID, root)->strVal;
//        VAR_TYPE type = getChild(OP_VAR_TYPE, root)->intVal;
//        s = addSymbol(id, type, depth, s);
//    } else if( root->type == OP_FUNCTION_FORWARD ) {
//        char *id = getChild(OP_ID, root)->strVal;
//        VAR_TYPE type = getChild(OP_FUNCTION_TYPE, root)->intVal;
//        s = addSymbol(id, type, depth, s);
//    }
//    root->symbols = s;
//    return s;
//    */
//    return NULL;
//}

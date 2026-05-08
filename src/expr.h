#ifndef expr_h
#define expr_h


#include "tokens.h"

typedef enum {
    EXPR_LITERAL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_GROUPING,
    EXPR_VARIABLE,
    EXPR_ASSIGN,
    EXPR_LOGICAL,
    EXPR_CALL
} ExprType;


typedef struct Expr Expr;


struct Expr {
    ExprType type;

    union {

        struct {
            Token value; /* NUMBER | STRING | true | false | nil */
        } literal;

        struct {
            Expr* left;
            Token operator;
            Expr* right;

        } binary;

        struct {
            Token operator;
            Expr* right;
        } unary;

        struct {
            Expr* expr;
        } grouping;

        struct {
            Token name;
        } variable;

        struct {
            Token name;
            Expr* expr;
        } assign;

        struct {
            Expr* left;
            Token op;
            Expr* right;
        } logical;

        struct {
            Expr* calle; /* the function that being called  */
            Expr** args; /* the function's arguments        */
            int arg_count;
            Token paren; /* closing ) for error reporting.  */
        } call;
        
    } ;
    

};

Expr* make_literal(Token value);
Expr* make_binary(Expr* left,Token operator,Expr* right);
Expr* make_unary(Token op, Expr* right);
Expr* make_grouping(Expr* expr);
Expr* make_variable(Token name);
Expr* make_assign(Token name,Expr* expr);
Expr* make_logical(Expr *left, Token op, Expr *right);
Expr* make_call(Expr* calle, Expr** args, int arg_count, Token paren);

char *print_expr(Expr* expr); 
void clear_expr(Expr* expr);


#endif
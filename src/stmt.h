#ifndef stmt_h
#define stmt_h

#include "expr.h"


typedef enum {
    STMT_LET,       // let x = 10;
    STMT_PRINT,     // print "hello moonscript"
    STMT_EXPR,      // 67 - 1 + 1
    STMT_BLOCK,     // { ... }
    STMT_IF,        // if true do smth
    STMT_WHILE,     // while true do smth
    STMT_FN,      // fn add(a, b) { ... }
    STMT_RET,     // ret value;

} StmtType; /* statement type */


typedef struct Stmt Stmt;

typedef struct { Expr *expression;              } ExpressionStmt;
typedef struct { Expr *expression;              } PrintStmt;
typedef struct { Token name; Expr *initializer; } LetStmt;
typedef struct { Stmt **statements; int count;  } BlockStmt;
typedef struct { Expr *condition;
                 Stmt *then_branch;
                 Stmt *else_branch; } IfStmt;

typedef struct { Expr *condition;
                 Stmt *body;        } WhileStmt;


typedef struct {
    Token  name;
    Token *params;      // array of parameter name tokens
    int    param_count;
    Stmt **body;        // array of statements
    int    body_count;
} FnStmt;


typedef struct {
    Token keyword;   // for error reporting
    Expr *value;     // NULL means ret nil
} RetStmt;


struct Stmt {

    StmtType type;

    union {
        ExpressionStmt expression;
        PrintStmt      print;
        LetStmt        let;
        BlockStmt      block;
        IfStmt         if_stmt;
        WhileStmt      while_stmt;
        FnStmt         fn;
        RetStmt        ret; 
    } as;


};

Stmt* make_expr_stmt(Expr* expr);
Stmt* make_print_stmt(Expr* expr);
Stmt* make_let_stmt(Token name,Expr* expr);
Stmt* make_block_stmt(Stmt** stmts, int count);
Stmt* make_if_stmt(Expr *cond, Stmt *then_branch, Stmt *else_branch);
Stmt* make_while_stmt(Expr *condition, Stmt *body);
Stmt* make_fn_stmt(Token name, Token *params, int param_count,Stmt **body, int body_count);
Stmt *make_ret_stmt(Token keyword, Expr *value);


void  clear_stmt(Stmt *stmt);


#endif
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

struct Stmt {

    StmtType type;

    union {
        ExpressionStmt expression;
        PrintStmt      print;
        LetStmt        let;
        BlockStmt      block;
        IfStmt         if_stmt;
        WhileStmt      while_stmt;
    } as;


};

Stmt* make_expr_stmt(Expr* expr);
Stmt* make_print_stmt(Expr* expr);
Stmt* make_let_stmt(Token name,Expr* expr);
Stmt* make_block_stmt(Stmt** stmts, int count);
Stmt *make_if_stmt(Expr *cond, Stmt *then_branch, Stmt *else_branch);
Stmt *make_while_stmt(Expr *condition, Stmt *body);


void  clear_stmt(Stmt *stmt);


#endif
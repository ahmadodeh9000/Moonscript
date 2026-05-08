

#include "stmt.h"
#include  "common.h"


/*================Constructors===============================*/

Stmt* make_expr_stmt(Expr* expr) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_EXPR;
    s->as.expression.expression = expr;

    return s;
}

Stmt* make_print_stmt(Expr* expr) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_PRINT;
    s->as.print.expression = expr;

    return s;

}

Stmt* make_let_stmt(Token name,Expr* expr) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_LET;
    s->as.let.name = name;
    s->as.let.initializer = expr;

    return s;
}

Stmt* make_block_stmt(Stmt** stmts, int count) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_BLOCK;
    s->as.block.statements = stmts;
    s->as.block.count = count;
    
    return s;
}

Stmt *make_if_stmt(Expr *cond, Stmt *then_branch, Stmt *else_branch){
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_IF;

    s->as.if_stmt.condition     = cond;
    s->as.if_stmt.then_branch   = then_branch;
    s->as.if_stmt.else_branch   = else_branch;

    return s;
}

Stmt *make_while_stmt(Expr *condition, Stmt *body) {
    Stmt *s = malloc(sizeof(Stmt));
    s->type = STMT_WHILE;
    s->as.while_stmt.condition = condition;
    s->as.while_stmt.body      = body;

    return s;
}

Stmt *make_fn_stmt(Token name, Token *params, int param_count,
                   Stmt **body, int body_count) {
    Stmt *s = malloc(sizeof(Stmt));
    s->type = STMT_FN;
    s->as.fn.name        = name;
    s->as.fn.params      = params;
    s->as.fn.param_count = param_count;
    s->as.fn.body        = body;
    s->as.fn.body_count  = body_count;
    return s;
}

Stmt *make_ret_stmt(Token keyword, Expr *value) {
    Stmt *s = malloc(sizeof(Stmt));
    s->type = STMT_RET;
    s->as.ret.keyword = keyword;
    s->as.ret.value   = value;
    return s;
}


/*==========CLEAR STATEMENTS==========================================================*/
void clear_stmt(Stmt *stmt) {
    if (!stmt) return;
    switch (stmt->type) {
        case STMT_EXPR:         clear_expr(stmt->as.expression.expression); break;
        case STMT_PRINT:        clear_expr(stmt->as.print.expression);      break;
        case STMT_LET:          clear_expr(stmt->as.let.initializer);       break;
        case STMT_BLOCK:
            for (int i = 0; i < stmt->as.block.count; i++)
                clear_stmt(stmt->as.block.statements[i]);
            free(stmt->as.block.statements);
            break;

        case STMT_IF:
            clear_expr(stmt->as.if_stmt.condition);
            clear_stmt(stmt->as.if_stmt.then_branch);
            if (stmt->as.if_stmt.else_branch)
                clear_stmt(stmt->as.if_stmt.else_branch);
            break;

        case STMT_WHILE:
            clear_expr(stmt->as.while_stmt.condition);
            clear_stmt(stmt->as.while_stmt.body);
            break;



        case STMT_FN:
            free(stmt->as.fn.params);
            for (int i = 0; i < stmt->as.fn.body_count; i++)
                clear_stmt(stmt->as.fn.body[i]);
            free(stmt->as.fn.body);
            break;

        case STMT_RET:
            clear_expr(stmt->as.ret.value);
            break;
    }
    free(stmt);
}
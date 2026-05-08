#include "expr.h"
#include "common.h"




Expr* make_literal(Token value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;

    if (value.type == TOKEN_NUMBER) {
        expr->literal.value.num_value = value.num_value;
    }

    else {
        expr->literal.value = value;
    }

    return expr;

}


Expr* make_binary(Expr* left,Token operator,Expr* right) {

    Expr* new_expr = malloc(sizeof(Expr));

    new_expr->type  = EXPR_BINARY;
    
    new_expr->binary.left       = left;
    new_expr->binary.operator   = operator;
    new_expr->binary.right      = right;


    return new_expr;


}

Expr* make_unary(Token op, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));

    expr->type = EXPR_UNARY;

    expr->unary.operator = op;
    expr->unary.right = right;

    return expr;
}


Expr* make_grouping(Expr* expr) {
    Expr* expr_ = malloc(sizeof(Expr));

    expr_->type = EXPR_GROUPING;
    expr_->grouping.expr = expr;


    return expr_;
}

Expr* make_variable(Token name) {
    Expr* expr_ = malloc(sizeof(Expr));

    expr_->type = EXPR_VARIABLE;
    expr_->variable.name = name;

    return expr_;
}

Expr* make_assign(Token name,Expr* expr) {
    Expr* expr_ = malloc(sizeof(Expr));

    expr_->type = EXPR_ASSIGN;
    expr_->assign.name = name;
    expr_->assign.expr = expr;


    return expr_;

}
Expr *make_logical(Expr *left, Token op, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_LOGICAL;
    e->logical.left  = left;
    e->logical.op    = op;
    e->logical.right = right;

    return e;
}

Expr *make_call(Expr *callee, Expr **args, int arg_count, Token paren) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_CALL;
    e->call.calle     = callee;
    e->call.args      = args;
    e->call.arg_count = arg_count;
    e->call.paren     = paren;

    return e;
}

void clear_expr(Expr* expr) {
    if (NULL == expr) {
        return;
    }

    switch (expr->type) {
        case EXPR_BINARY: {
            clear_expr(expr->binary.left);
            clear_expr(expr->binary.right);
            break;
        }

        case EXPR_UNARY: {
            clear_expr(expr->unary.right);
            break;
        }

        case EXPR_GROUPING: {
            clear_expr(expr->grouping.expr);
            break;

        }

        case EXPR_ASSIGN: {
            clear_expr(expr->assign.expr);
            break;
        }

        case EXPR_LOGICAL: {
            clear_expr(expr->logical.left);
            clear_expr(expr->logical.right);
            break;
        }

        case EXPR_CALL: {
            for (int i = 0; i < expr->call.arg_count; ++i) {
                clear_expr(expr->call.args[i]);
            }
            clear_expr(expr->call.calle);

            break;
        }

        case EXPR_LITERAL:  break;
        case EXPR_VARIABLE: break;

        default: break;

    }

    free(expr);

    expr = NULL;

#ifdef DEBUG__
    printf("cleared\n");
#endif

}

char *print_expr(Expr *expr) {
    switch (expr->type) {
        case EXPR_LITERAL: {
            
            if (expr->literal.value.type == TOKEN_NUMBER) {
                char* result = malloc(32);

                sprintf(result,"%g",expr->literal.value.num_value);

                return result;

            }


            char* result = malloc(expr->literal.value.length + 1);
            memcpy(result,expr->literal.value.start, expr->literal.value.length);
            result[expr->literal.value.length] = '\0';

            return result;
        }

        case EXPR_BINARY: {
            char *left  = print_expr(expr->binary.left);
            char *right = print_expr(expr->binary.right);
            Token op = expr->binary.operator;

            char *result = malloc(strlen(left) + strlen(right) + op.length + 10);
            

            sprintf (
                result,
                "(%.*s %s %s)",
                op.length,
                op.start,
                left,
                right
            );



            return result;
        }

        case EXPR_UNARY: {
            char* right = print_expr(expr->unary.right);
            Token op = expr->unary.operator;

            char* result = malloc(strlen(right) + op.length + 10);

            sprintf(
                result,
                "(%.*s %s)",
                op.length,
                op.start,
                right
            );

            return result;
        }


        case EXPR_VARIABLE: {
            char* result = malloc(expr->variable.name.length + 1);
            memcpy(result,expr->variable.name.start, expr->variable.name.length);
            result[expr->variable.name.length] = '\0';

            return result;
        }


        case EXPR_ASSIGN: {
            char* value = print_expr(expr->assign.expr);

            char* result = malloc(
                expr->assign.name.length +strlen(value) + 20
            );

            sprintf(
                result,
                "(assign %.*s %s)",
                expr->assign.name.length,
                expr->assign.name.start,
                value
            );

            free(value);
            return result;
        }

        case EXPR_GROUPING:{
            char* inner = print_expr(expr->grouping.expr);

            char* result = malloc(strlen(inner) + 20);


            sprintf(
                result,
                "(group %s)",
                inner
            );

            return result;
        }
    
    
        case EXPR_LOGICAL: {
            char *left  = print_expr(expr->logical.left);
            char *right = print_expr(expr->logical.right);
            char *result = malloc(strlen(left) + strlen(right) + 10);
            sprintf(result, "(%.*s %s %s)",
                expr->logical.op.length,
                expr->logical.op.start,
                left, right);
            free(left);
            free(right);
            return result;
        }


        case EXPR_CALL: {
            char *callee = print_expr(expr->call.calle);
            char *result = malloc(strlen(callee) + 64);
            sprintf(result, "(call %s [%d args])", callee, expr->call.arg_count);
            free(callee);
            return result;
        }
    }
}
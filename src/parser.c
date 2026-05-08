#include "parser.h"
#include "common.h"

void init_parser(Parser* parser, Token* tokens, int count) {
    if (NULL == parser) {
        fprintf(stderr, "THE PARSER IS NULL");
        exit(67);
    }

    parser->tokens = tokens;
    parser->count = count;
    parser->current = 0;
    parser->had_error = false;
}

/*===========================================================================================*/
/*=====================THE PARSER LOGIC AND SOME HELPER FUNCTIONS============================*/
/*===========================================================================================*/

static Token peek(Parser* p);
static Token prev(Parser* p);
static bool at_the_end(Parser* p);
static Token advance(Parser* p);
static bool check(Parser* p, TokenType ty);
static bool match(Parser* p, TokenType* tys, int count);

static Expr* expression(Parser *p);
static Expr* assignment(Parser *p);
static Expr* equality(Parser *p);
static Expr* comparison(Parser *p);
static Expr* term(Parser *p);
static Expr* factor(Parser *p);
static Expr* unary(Parser *p);
static Expr* primary(Parser *p);
static Expr *logic_or(Parser *p) ;
static Expr *logic_and(Parser *p);
static Expr *call(Parser *p);
static Expr *finish_call(Parser *p, Expr *callee);

static Stmt *statement(Parser *p);
static Stmt *declaration(Parser *p);
static Stmt *print_statement(Parser *p);
static Stmt *expression_statement(Parser *p);
static Stmt *let_declaration(Parser *p);
static Stmt *block_statement(Parser *p);
static Stmt *if_statement(Parser *p);
static Stmt *while_statement(Parser *p);
static Stmt *for_statement(Parser *p);
static Stmt *fn_declaration(Parser *p);
static Stmt *ret_statement(Parser *p);



/*===========================================================================================*/
/*===================================HELPER FUNCTIONS========================================*/
/*===========================================================================================*/

static Token peek(Parser* p) {
    return p->tokens[p->current];
}

static Token prev(Parser* p) {
    return p->tokens[p->current - 1];
}

static bool at_the_end(Parser* p) {
    return peek(p).type == TOKEN_EOF;
}

static Token advance(Parser* p) {
    if (!at_the_end(p)) p->current++;
    return prev(p);
}

static bool check(Parser* p, TokenType ty) {
    return at_the_end(p) ? false : (peek(p).type == ty);
}

static bool match(Parser* p, TokenType* tys, int count) {
    for (int i = 0; i < count; ++i) {
        if (check(p, tys[i])) {
            advance(p);
            return true;
        }
    }

    return false;
}

#define MATCH(parser, ...) \
    match(parser, (TokenType[]){__VA_ARGS__}, \
    sizeof((TokenType[]){__VA_ARGS__}) / sizeof(TokenType))

/*===========================================================================================*/
/*===================================ERROR HANDLING==========================================*/
/*===========================================================================================*/

static void parser_error(Parser* p, Token tok, const char* msg) {
    p->had_error = true;

    if (TOKEN_EOF == tok.type) {
        fprintf(
            stderr,
            "[line %d] error at end: %s\n",
            tok.line,
            msg
        );
    } else {
        fprintf(
            stderr,
            "[line %d] error at %.*s: %s\n",
            tok.line,
            tok.length,
            tok.start,
            msg
        );
    }
}

static Token consume(Parser *p, TokenType type, const char *msg) {
    if (check(p, type)) return advance(p);
    parser_error(p, peek(p), msg);
    return peek(p);
}

/*===========================================================================================*/
/*===================================EXPRESSIONS=============================================*/
/*===========================================================================================*/

static Expr *expression(Parser *p) {
    return assignment(p);
}

static Expr *assignment(Parser *p) {
    Expr *expr = logic_or(p);   // ← changed

    if (MATCH(p, TOKEN_ASSIGN)) {
        Token eq    = prev(p);
        Expr *value = assignment(p);
        if (expr->type == EXPR_VARIABLE) {
            return make_assign(expr->variable.name, value);
        }
        parser_error(p, eq, "Invalid assignment target.");
    }

    return expr;
}

static Expr *call(Parser *p) {
    Expr *expr = primary(p);

    // keep consuming calls as long as we see (
    while (MATCH(p, TOKEN_LPAREN)) {
        expr = finish_call(p, expr);
    }

    return expr;
}

static Expr *finish_call(Parser *p, Expr *callee) {
    int   capacity = 8, count = 0;
    Expr **args = malloc(capacity * sizeof(Expr*));

    if (!check(p, TOKEN_RPAREN)) {
        do {
            if (count >= 255) {
                parser_error(p, peek(p), "Too many arguments.");
            }
            if (count >= capacity) {
                capacity *= 2;
                args = realloc(args, capacity * sizeof(Expr*));
            }
            args[count++] = expression(p);
        } while (MATCH(p, TOKEN_COMMA));
    }

    Token paren = consume(p, TOKEN_RPAREN, "Expect ')' after arguments.");
    return make_call(callee, args, count, paren);
}

static Expr *logic_or(Parser *p) {
    Expr *expr = logic_and(p);

    while (MATCH(p, TOKEN_OR)) {
        Token op    = prev(p);
        Expr *right = logic_and(p);
        expr = make_logical(expr, op, right);
    }

    return expr;
}

static Expr *logic_and(Parser *p) {
    Expr *expr = equality(p);

    while (MATCH(p, TOKEN_AND)) {
        Token op    = prev(p);
        Expr *right = equality(p);
        expr = make_logical(expr, op, right);
    }

    return expr;
}

static Expr *equality(Parser *p) {
    Expr *expr = comparison(p);

    while (MATCH(p, TOKEN_NEQ, TOKEN_EQ)) {
        Token op = prev(p);
        Expr *right = comparison(p);
        expr = make_binary(expr, op, right);
    }

    return expr;
}

static Expr *comparison(Parser *p) {
    Expr *expr = term(p);

    while (MATCH(p, TOKEN_GT, TOKEN_GTE, TOKEN_LT, TOKEN_LTE)) {
        Token op = prev(p);
        Expr *right = term(p);
        expr = make_binary(expr, op, right);
    }

    return expr;
}

static Expr *term(Parser *p) {
    Expr *expr = factor(p);

    while (MATCH(
        p,
        TOKEN_MINUS,
        TOKEN_HASHTAG,
        TOKEN_PLUS,
        TOKEN_PERCENT,
        TOKEN_LOGICAL_AND,
        TOKEN_LOGICAL_OR,
        TOKEN_LOGICAL_XOR
    )) {
        Token op = prev(p);
        Expr *right = factor(p);
        expr = make_binary(expr, op, right);
    }

    return expr;
}

static Expr *factor(Parser *p) {
    Expr *expr = unary(p);

    while (MATCH(p, TOKEN_SLASH, TOKEN_STAR)) {
        Token op = prev(p);
        Expr *right = unary(p);
        expr = make_binary(expr, op, right);
    }

    return expr;
}

static Expr *unary(Parser *p) {
    if (MATCH(p, TOKEN_BANG, TOKEN_MINUS)) {
        Token op = prev(p);
        Expr *right = unary(p);
        return make_unary(op, right);
    }

    return call(p);
}

static Expr *primary(Parser *p) {
    if (MATCH(p, TOKEN_FALSE))  return make_literal(prev(p));
    if (MATCH(p, TOKEN_TRUE))   return make_literal(prev(p));
    if (MATCH(p, TOKEN_NIL))    return make_literal(prev(p));
    if (MATCH(p, TOKEN_NUMBER)) return make_literal(prev(p));
    if (MATCH(p, TOKEN_STRING)) return make_literal(prev(p));

    if (MATCH(p, TOKEN_IDENT))
        return make_variable(prev(p));

    if (MATCH(p, TOKEN_LPAREN)) {
        Expr *expr = expression(p);
        consume(p, TOKEN_RPAREN, "Expect ')' after expression.");
        return make_grouping(expr);
    }

    parser_error(p, peek(p), "Expect expression.");
    return NULL;
}

/*===========================================================================================*/
/*===================================STATEMENTS==============================================*/
/*===========================================================================================*/

static Stmt *statement(Parser *p) {
    if (MATCH(p, TOKEN_RET))    return ret_statement(p); 
    if (MATCH(p, TOKEN_IF))     return if_statement(p);
    if (MATCH(p, TOKEN_WHILE))  return while_statement(p);
    if (MATCH(p, TOKEN_FOR))    return for_statement(p);
    if (MATCH(p, TOKEN_PRINT))  return print_statement(p);
    if (MATCH(p, TOKEN_LBRACE)) return block_statement(p);


    return expression_statement(p);
}

static Stmt *print_statement(Parser *p) {
    Expr *value = expression(p);
    consume(p, TOKEN_SEMICOLON, "Expect ';' after value.");
    return make_print_stmt(value);
}

static Stmt *expression_statement(Parser *p) {
    Expr *expr = expression(p);
    consume(p, TOKEN_SEMICOLON, "Expect ';' after expression.");
    return make_expr_stmt(expr);
}

static Stmt *let_declaration(Parser *p) {
    Token name = consume(p, TOKEN_IDENT, "Expect variable name.");

    Expr *initializer = NULL;

    if (MATCH(p, TOKEN_ASSIGN))
        initializer = expression(p);

    consume(p, TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

    return make_let_stmt(name, initializer);
}

static Stmt *block_statement(Parser *p) {
    int capacity = 8;
    int count = 0;

    Stmt **stmts = malloc(capacity * sizeof(Stmt*));

    while (!check(p, TOKEN_RBRACE) && !at_the_end(p)) {
        if (count >= capacity) {
            capacity *= 2;
            stmts = realloc(stmts, capacity * sizeof(Stmt*));
        }

        stmts[count++] = declaration(p);
    }

    consume(p, TOKEN_RBRACE, "Expect '}' after block.");

    return make_block_stmt(stmts, count);
}



static Stmt *if_statement(Parser *p) {
    consume(p, TOKEN_LPAREN, "Expect '(' after 'if'.");
    Expr *condition = expression(p);
    consume(p, TOKEN_RPAREN, "Expect ')' after condition.");

    Stmt *then_branch = statement(p);
    Stmt *else_branch = NULL;

    if (MATCH(p, TOKEN_ELSE))
        else_branch = statement(p);

    return make_if_stmt(condition, then_branch, else_branch);
}

static Stmt *while_statement(Parser *p) {
    consume(p, TOKEN_LPAREN, "Expect '(' after 'while'.");
    Expr *condition = expression(p);
    consume(p, TOKEN_RPAREN, "Expect ')' after condition.");
    Stmt *body = statement(p);
    return make_while_stmt(condition, body);
}


static Stmt *for_statement(Parser *p) {
    consume(p, TOKEN_LPAREN, "Expect '(' after 'for'.");

    // initializer: let i = 0; OR i = 0; OR nothing
    Stmt *initializer = NULL;
    if (MATCH(p, TOKEN_SEMICOLON)) {
        initializer = NULL;
    } else if (MATCH(p, TOKEN_LET)) {
        initializer = let_declaration(p);
    } else {
        initializer = expression_statement(p);
    }

    // condition: i < 10
    Expr *condition = NULL;
    if (!check(p, TOKEN_SEMICOLON))
        condition = expression(p);
    consume(p, TOKEN_SEMICOLON, "Expect ';' after loop condition.");

    // increment: i = i + 1
    Expr *increment = NULL;
    if (!check(p, TOKEN_RPAREN))
        increment = expression(p);
    consume(p, TOKEN_RPAREN, "Expect ')' after for clauses.");

    Stmt *body = statement(p);

    // desugar — build from the inside out

    // 1. append increment to body
    if (increment != NULL) {
        Stmt **stmts = malloc(2 * sizeof(Stmt*));
        stmts[0] = body;
        stmts[1] = make_expr_stmt(increment);
        body = make_block_stmt(stmts, 2);
    }

    // 2. wrap in while
    if (condition == NULL) {
        // no condition = infinite loop, use "true"
        Token t = {TOKEN_TRUE, "true", 4, 0, 0};
        condition = make_literal(t);
    }
    body = make_while_stmt(condition, body);

    // 3. wrap initializer around the while
    if (initializer != NULL) {
        Stmt **stmts = malloc(2 * sizeof(Stmt*));
        stmts[0] = initializer;
        stmts[1] = body;
        body = make_block_stmt(stmts, 2);
    }

    return body;
}


static Stmt *fn_declaration(Parser *p) {
    Token name = consume(p, TOKEN_IDENT, "Expect function name.");
    consume(p, TOKEN_LPAREN, "Expect '(' after function name.");

    // parse parameters
    int    capacity = 8, count = 0;
    Token *params = malloc(capacity * sizeof(Token));

    if (!check(p, TOKEN_RPAREN)) {
        do {
            if (count >= 255) parser_error(p, peek(p), "Too many parameters.");
            if (count >= capacity) {
                capacity *= 2;
                params = realloc(params, capacity * sizeof(Token));
            }
            params[count++] = consume(p, TOKEN_IDENT, "Expect parameter name.");
        } while (MATCH(p, TOKEN_COMMA));
    }
    consume(p, TOKEN_RPAREN, "Expect ')' after parameters.");
    consume(p, TOKEN_LBRACE, "Expect '{' before function body.");

    // parse the function body block inline (same logic as block_statement)
    int    body_capacity = 8, body_count = 0;
    Stmt **body = malloc(body_capacity * sizeof(Stmt*));

    while (!check(p, TOKEN_RBRACE) && !at_the_end(p)) {
        if (body_count >= body_capacity) {
            body_capacity *= 2;
            body = realloc(body, body_capacity * sizeof(Stmt*));
        }
        body[body_count++] = declaration(p);
    }

    consume(p, TOKEN_RBRACE, "Expect '}' after function body.");

    return make_fn_stmt(name, params, count, body, body_count);
}

static Stmt *ret_statement(Parser *p) {
    Token keyword = prev(p);
    Expr *value   = NULL;

    if (!check(p, TOKEN_SEMICOLON))
        value = expression(p);

    consume(p, TOKEN_SEMICOLON, "Expect ';' after return value.");
    return make_ret_stmt(keyword, value);
}

static Stmt *declaration(Parser *p) {
    if (MATCH(p, TOKEN_FUN)) return fn_declaration(p);  
    if (MATCH(p, TOKEN_LET)) return let_declaration(p);
    return statement(p);
}

/*===========================================================================================*/
/*===================================MAIN PARSE==============================================*/
/*===========================================================================================*/

Stmt **parse(Parser *p, int *out_count) {
    int capacity = 8;
    int count = 0;

    Stmt **stmts = malloc(capacity * sizeof(Stmt*));

    while (!at_the_end(p)) {
        if (count >= capacity) {
            capacity *= 2;
            stmts = realloc(stmts, capacity * sizeof(Stmt*));
        }

        stmts[count++] = declaration(p);
    }

    *out_count = count;
    return stmts;
}
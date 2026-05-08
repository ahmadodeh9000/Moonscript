


#include "interpreter.h"
#include "common.h"
#include "fn.h"




double power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) return 1.0 / power(base, -exp); // Handle negative exponents

    double half = power(base, exp / 2);
    if (exp % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}


/*============DECLARING HELPER FUNCTIONS========================*/
static Value evaluate(Expr *expr,Interpreter* current_interp);
static void execute(Interpreter *interp, Stmt *stmt);
static bool  is_truthy(Value v);
static bool  is_equal(Value a, Value b);
static void  runtime_error(Token op, const char *msg);

static bool is_truthy(Value v) {
    if (IS_NIL(v))  return false;
    if (IS_BOOL(v)) return AS_BOOL(v);

    return 1;   /* numbers and strings are always true */
}


static bool is_equal(Value a, Value b) {
    if (a.type != b.type) return false;


    switch(a.type) {
        case VAL_NIL:       return true;
        case VAL_NUMBER:    return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_STRING:    return strcmp(AS_STRING(a), AS_STRING(b)) == 0;
        case VAL_BOOL:      return AS_BOOL(a) == AS_BOOL(b);
        default: break;
    }

    return false;
}

static void runtime_error(Token op, const char *msg) {
    fprintf(stderr, "[line %d] Runtime error at '%.*s': %s\n",
            op.line, op.length, op.start, msg);
    exit(70);
}

static void check_number(Token op, Value v) {
    if (!IS_NUMBER(v))
        runtime_error(op, "Operand must be a number.");
}

static void check_numbers(Token op, Value a, Value b) {
    if (!IS_NUMBER(a) || !IS_NUMBER(b))
        runtime_error(op, "Operands must be numbers.");
}

/* shit but works */
static Value make_string_value(const char *start, int length) {
    char *s = malloc(length + 1);
    memcpy(s, start, length);
    s[length] = '\0';
    return STRING_VAL(s);
}


static Value evaluate(Expr *expr,Interpreter* current_interp) {
    switch (expr->type) {

        /*=====LITERAL==============================================*/
        case EXPR_LITERAL: {
            Token t = expr->literal.value;

            switch(t.type) {
                case TOKEN_NUMBER:  return NUMBER_VAL(t.num_value);
                case TOKEN_STRING:  return make_string_value(t.start, t.length);;
                case TOKEN_FALSE:   return BOOL_VAL(false);
                case TOKEN_TRUE:    return BOOL_VAL(true);
                case TOKEN_NIL:     return NIL_VAL;
                default:            return NIL_VAL;
            }

        }

        /*===========GROUPING======================================*/
        case EXPR_GROUPING: {
            return evaluate(expr->grouping.expr,current_interp);
        }


        /*=============UNARY========================================*/
        case EXPR_UNARY: {
            Value r     = evaluate(expr->unary.right,current_interp);
            Token op    = expr->unary.operator;


            switch (op.type) {

                case TOKEN_MINUS:
                    check_number(op,r);
                    return NUMBER_VAL(-AS_NUMBER(r));

                case TOKEN_BANG:
                    return BOOL_VAL(!is_truthy(r));

                default: return NIL_VAL;

            }

        }


        case EXPR_VARIABLE:
            
            return env_get(current_interp->env, expr->variable.name);

            

        case EXPR_ASSIGN: {
            Value val = evaluate(expr->assign.expr,current_interp);
            env_assign(current_interp->env, expr->assign.name, val);
            return val;
        }

        case EXPR_BINARY: {

            Value l = evaluate(expr->binary.left,current_interp);
            Value r = evaluate(expr->binary.right, current_interp);
            Token op= expr->binary.operator;


            switch(op.type) {

                case TOKEN_PLUS:
                    if (IS_NUMBER(r) && IS_NUMBER(l)) {
                        return NUMBER_VAL(AS_NUMBER(r) + AS_NUMBER(l));
                    }

                    if (IS_STRING(r) && IS_STRING(l)) {                        
                        int   la = strlen(AS_STRING(l));
                        int   lb = strlen(AS_STRING(r));
                        char *s  = malloc(la + lb + 1);

                       // fprintf(stderr, "concat: l='%s' (%d) r='%s' (%d)\n",
                        //AS_STRING(l), la, AS_STRING(r), lb);

                        memcpy(s, AS_STRING(l), la);
                        memcpy(s + la, AS_STRING(r), lb);
                        s[la + lb] = '\0';
                       // fprintf(stderr, "result: '%s'\n", s);
                        return STRING_VAL(s);

                    }


                    if (IS_NUMBER(l) && IS_STRING(r)) {
                        char num_str[20];
                        snprintf(num_str, sizeof(num_str), "%g", AS_NUMBER(l));

                        int num_len = strlen(num_str);
                        int str_len = strlen(AS_STRING(r));

                        char* res = malloc(num_len + str_len + 1);

                        memcpy(res, num_str, num_len);
                        memcpy(res + num_len, AS_STRING(r), str_len);

                        res[num_len + str_len] = '\0';

                        return STRING_VAL(res);
                    }

                    if (IS_STRING(l) && IS_NUMBER(r)) {
                        char num_str[20];
                        snprintf(num_str, sizeof(num_str), "%g", AS_NUMBER(r));

                        int str_len = strlen(AS_STRING(l));
                        int num_len = strlen(num_str);

                        char* res = malloc(str_len + num_len + 1);

                        memcpy(res, AS_STRING(l), str_len);
                        memcpy(res + str_len, num_str, num_len);

                        res[str_len + num_len] = '\0';

                        return STRING_VAL(res);
                    }

                    runtime_error(op, "Operands must be two numbers or two strings.");

                
                case TOKEN_MINUS:
                    check_numbers(op,l,r);
                    return NUMBER_VAL(AS_NUMBER(l) - AS_NUMBER(r));

                case TOKEN_STAR:
                    check_numbers(op, l, r);
                    return NUMBER_VAL(AS_NUMBER(l) * AS_NUMBER(r));

                case TOKEN_SLASH:
                    check_numbers(op,l,r);

                    if (AS_NUMBER(r) == 0) {
                        runtime_error(op,"Divide by zero !");
                    }

                    return NUMBER_VAL(AS_NUMBER(l) / AS_NUMBER(r));
                
                case TOKEN_PERCENT:
                    check_numbers(op,l,r);
                    return NUMBER_VAL((double)((long)AS_NUMBER(l) % (long)AS_NUMBER(r)));

                case TOKEN_HASHTAG: {
                    check_numbers(op,l,r);

                    double base = AS_NUMBER(l);
                    int exp     = (int) AS_NUMBER(r);
                    
                    double result = power(base,exp);

                    return NUMBER_VAL(result);

                }

                
                case TOKEN_GT:
                    check_numbers(op,l,r);

                    return BOOL_VAL(AS_NUMBER(l) > AS_NUMBER(r));

                case TOKEN_GTE:
                    check_numbers(op,l,r);

                    return BOOL_VAL(AS_NUMBER(l) >= AS_NUMBER(r));

                case TOKEN_LT:
                    check_numbers(op,l,r);

                    return BOOL_VAL(AS_NUMBER(l) < AS_NUMBER(r));

                case TOKEN_LTE:
                    check_numbers(op,l,r);

                    return BOOL_VAL(AS_NUMBER(l) <= AS_NUMBER(r));

                case TOKEN_EQ:  return BOOL_VAL( is_equal(l, r));
                case TOKEN_NEQ: return BOOL_VAL(!is_equal(l, r));


                case TOKEN_LOGICAL_AND:
                    check_numbers(op,l,r);
                    return NUMBER_VAL((double)((long)AS_NUMBER(l) & (long) AS_NUMBER(r)));

                case TOKEN_LOGICAL_OR:
                    check_numbers(op,l,r);
                    return NUMBER_VAL((double)((long)AS_NUMBER(l) | (long) AS_NUMBER(r)));

                case TOKEN_LOGICAL_XOR:
                    check_numbers(op,l,r);
                    return NUMBER_VAL((double)((long)AS_NUMBER(l) ^ (long) AS_NUMBER(r)));


                default: return NIL_VAL;

            }


        }

        case EXPR_LOGICAL: {
            Value left = evaluate(expr->logical.left, current_interp);

            if (expr->logical.op.type == TOKEN_OR) {
                if (is_truthy(left)) return left;
            } else {
                if (!is_truthy(left)) return left;
            }

            // only evaluate right if we have to
            return evaluate(expr->logical.right, current_interp);
        }


        case EXPR_CALL: {
            Value callee = evaluate(expr->call.calle, current_interp);

            if (!IS_FN(callee)) {
                runtime_error(expr->call.paren, "Can only call functions.");
            }

            MSFn *fn = AS_FN(callee);


            if (expr->call.arg_count != fn->param_count) {
                fprintf(stderr, "Expected %d arguments but got %d.\n",
                        fn->param_count, expr->call.arg_count);
                exit(70);
            }

                        // evaluate arguments
            Value *args = malloc(fn->param_count * sizeof(Value));
            for (int i = 0; i < expr->call.arg_count; i++)
                args[i] = evaluate(expr->call.args[i], current_interp);

            // new scope — parent is the CLOSURE, not current env
            Environment *fn_env = make_environment(fn->closure);

            // bind parameters to arguments
            for (int i = 0; i < fn->param_count; i++)
                env_define(fn_env,
                           fn->params[i].start,
                           fn->params[i].length,
                           args[i]);
            free(args);


                        // save state
            Environment *prev_env = current_interp->env;
            jmp_buf      prev_jump;
            memcpy(prev_jump, current_interp->ret_jump, sizeof(jmp_buf));
                    
            int prev_returning = current_interp->returning;

            current_interp->env       = fn_env;
            current_interp->returning = 0;

            Value result = NIL_VAL;


            if (setjmp(current_interp->ret_jump) == 0) {
                // run body normally
                for (int i = 0; i < fn->body_count; i++)
                    execute(current_interp, fn->body[i]);
            } else {
                // ret was called — longjmp lands here
                result = current_interp->ret_value;
            }

            // restore state
            current_interp->env = prev_env;
            current_interp->returning = prev_returning;
            memcpy(current_interp->ret_jump, prev_jump, sizeof(jmp_buf));

            free_environment(fn_env);
            return result;

        }


    }


    return NIL_VAL;
}


/*=========MAIN LOGIC=====================*/

static void print_value(Value v) {
    switch (v.type) {
        case VAL_NIL:    printf("NIL\n"); break;
        case VAL_BOOL:   printf("%s\n", AS_BOOL(v) ? "true" : "false"); break;
        case VAL_NUMBER: {
            double n = AS_NUMBER(v);
            if (n == (long)n)
                printf("%ld\n", (long)n);   // print 3 not 3.0
            else
                printf("%g\n", n);
            break;
        }
        case VAL_STRING: printf("%s\n", AS_STRING(v)); break;

        case VAL_FN:
            printf("<fn %.*s>\n",
                   AS_FN(v)->name.length,
                   AS_FN(v)->name.start);
            break;
    }
}


Interpreter*  init_interpreter() {
    Interpreter* interp = malloc(sizeof(Interpreter));

    interp->env = make_environment(NULL);
#ifdef __debug1
    fprintf(stderr, "DEBUG init: interp=%p env=%p\n", interp, interp->env);
#endif
    return interp;
}



static void execute(Interpreter *interp, Stmt *stmt) {

#ifdef __debug1
    fprintf(stderr, "DEBUG execute: interp=%p env=%p\n", interp, interp->env);
#endif
    switch (stmt->type) {

        case STMT_EXPR:
            evaluate(stmt->as.expression.expression,interp);
            break;

        case STMT_PRINT: {
            Value val = evaluate(stmt->as.print.expression,interp);
            print_value(val);
            break;
        }

        case STMT_LET: {
            Value val = NIL_VAL;
            if (stmt->as.let.initializer)
                val = evaluate(stmt->as.let.initializer,interp);
            env_define(interp->env,
                       stmt->as.let.name.start,
                       stmt->as.let.name.length,
                       val);
            break;
        }

        case STMT_BLOCK: {
            // new scope
            Environment *previous = interp->env;
            interp->env = make_environment(previous);

            for (int i = 0; i < stmt->as.block.count; i++)
                execute(interp, stmt->as.block.statements[i]);

            free_environment(interp->env);
            interp->env = previous;  // restore
            break;
        }


        case STMT_IF: {
            Value condition = evaluate(stmt->as.if_stmt.condition, interp);
            if (is_truthy(condition)) {
                execute(interp, stmt->as.if_stmt.then_branch);
            } else if (stmt->as.if_stmt.else_branch != NULL) {
                execute(interp, stmt->as.if_stmt.else_branch);
            }
            break;
        }

        case STMT_WHILE: {
            while (is_truthy(evaluate(stmt->as.while_stmt.condition, interp))) {
                execute(interp, stmt->as.while_stmt.body);
            }
            break;
        }


        case STMT_FN: {
            MSFn  *fn  = make_msfn(&stmt->as.fn, interp->env);
            Value  val = FN_VAL(fn);
            env_define(interp->env,
                       stmt->as.fn.name.start,
                       stmt->as.fn.name.length,
                       val);
            break;
        }

        case STMT_RET: {
            Value val = NIL_VAL;
            if (stmt->as.ret.value)
                val = evaluate(stmt->as.ret.value, interp);
            interp->ret_value = val;
            longjmp(interp->ret_jump, 1);
            break;
        }

    }
}

void interpret(Interpreter *interp, Stmt **stmts, int count) {
    for (int i = 0; i < count; i++) {
        execute(interp, stmts[i]);
    }

}


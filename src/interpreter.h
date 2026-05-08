#ifndef interpreter_h
#define interpreter_h

#include "expr.h"
#include "value.h"
#include "stmt.h"
#include "environment.h"

#include <setjmp.h>

typedef struct {
    Environment *env;

    // return mechanism
    jmp_buf      ret_jump;
    Value        ret_value;
    int          returning;
} Interpreter;

Interpreter*  init_interpreter();
void  interpret(Interpreter *interp, Stmt **stmts, int count);



#endif
#ifndef interpreter_h
#define interpreter_h

#include "expr.h"
#include "value.h"
#include "stmt.h"
#include "environment.h"

typedef struct {
    Environment *env;
} Interpreter;

Interpreter*  init_interpreter();
void  interpret(Interpreter *interp, Stmt **stmts, int count);



#endif
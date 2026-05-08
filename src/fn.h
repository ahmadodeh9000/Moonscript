#ifndef fn_h
#define fn_h



#include "stmt.h"
#include "environment.h"

typedef struct MSFn {
    Token       name;
    Token      *params;
    int         param_count;
    Stmt      **body;
    int         body_count;
    Environment *closure;   // the scope where the fn was DEFINED
} MSFn;


MSFn *make_msfn(FnStmt *decl, Environment *closure);



#endif
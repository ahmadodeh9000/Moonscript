

#include "fn.h"
#include "common.h"

MSFn *make_msfn(FnStmt *decl, Environment *closure) {
    MSFn *fn = malloc(sizeof(MSFn));
    fn->name        = decl->name;
    fn->params      = decl->params;
    fn->param_count = decl->param_count;
    fn->body        = decl->body;
    fn->body_count  = decl->body_count;
    fn->closure     = closure;
    if (closure) closure->ref_count++;   // ← fn holds a reference
    return fn;
}
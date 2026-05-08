#ifndef environment_h
#define environment_h

#include "value.h"
#include "tokens.h"

#define ENV_CAPACITY 64

typedef struct {
    char* key;
    Value val;
} Entry;

typedef struct Environment Environment;

struct Environment {
    Entry        entries[ENV_CAPACITY];
    int          count;
    Environment *enclosing;   // parent scope
};

Environment *make_environment(Environment *enclosing);
void         free_environment(Environment *env);
void         env_define(Environment *env, const char *name, int len, Value value);
Value        env_get(Environment *env, Token name);
void         env_assign(Environment *env, Token name, Value value);

#endif
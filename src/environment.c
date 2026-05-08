

#include "environment.h"
#include "common.h"

static bool names_equal(const char *a, const char *b, int blen) {
    int alen = strlen(a);
    if (alen != blen) return false;
    return memcmp(a, b, blen) == 0;
}


Environment *make_environment(Environment *enclosing) {
    Environment* env = malloc(sizeof(Environment));

    env->count      = 0;
    env->enclosing  = enclosing;
    memset(env->entries,0,sizeof(env->entries));


    return env;

}

void free_environment(Environment *env) {
    for (int i = 0; i < env->count; i++) free(env->entries[i].key);

    free(env);
}

void env_define(Environment *env, const char *name, int len, Value value) {
    // update if already exists
#ifdef __debug1
    fprintf(stderr, "DEBUG env_define: env=%p count=%d name='%.*s'\n",
            env, env->count, len, name);
#endif
    for (int i = 0; i < env->count; i++) {
        if (strncmp(env->entries[i].key, name, len) == 0
            && (int)strlen(env->entries[i].key) == len) {
            env->entries[i].val = value;
            return;
        }
    }
    // add new
    char *key = malloc(len + 1);
    memcpy(key, name, len);
    key[len] = '\0';
    env->entries[env->count].key   = key;
    env->entries[env->count].val = value;
    env->count++;
}



Value env_get(Environment *env, Token name) {
    for (int i = 0; i < env->count; i++) {
        if (strncmp(env->entries[i].key, name.start, name.length) == 0
            && (int)strlen(env->entries[i].key) == name.length) {
            return env->entries[i].val;
        }
    }
    // not found — try enclosing scope
    if (env->enclosing)
        return env_get(env->enclosing, name);

    fprintf(stderr, "[line %d] Undefined variable '%.*s'.\n",
            name.line, name.length, name.start);

    exit(70);
}



void env_assign(Environment *env, Token name, Value value) {
    for (int i = 0; i < env->count; i++) {
        if (strncmp(env->entries[i].key, name.start, name.length) == 0
            && (int)strlen(env->entries[i].key) == name.length) {
            env->entries[i].val = value;
            return;
        }
    }
    // not found — try enclosing scope
    if (env->enclosing) {
        env_assign(env->enclosing, name, value);
        return;
    }

    fprintf(stderr, "[line %d] Undefined variable '%.*s'.\n",
            name.line, name.length, name.start);

    printf("assign works");
    exit(70);
}
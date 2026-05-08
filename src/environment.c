#include "environment.h"
#include "common.h"

static bool names_equal(const char *a, const char *b, int blen) {
    int alen = strlen(a);
    if (alen != blen) return false;
    return memcmp(a, b, blen) == 0;
}

Environment *make_environment(Environment *enclosing) {
    Environment *env = malloc(sizeof(Environment));
    env->count     = 0;
    env->enclosing = enclosing;
    env->ref_count = 1;                        // starts at 1 for the owner
    if (enclosing) enclosing->ref_count++;     // parent gains a reference
    memset(env->entries, 0, sizeof(env->entries));
    return env;
}

void free_environment(Environment *env) {
    if (!env) return;

    env->ref_count--;
    if (env->ref_count > 0) return;   // still referenced, don't free yet

    // actually free
    for (int i = 0; i < env->count; i++)
        free(env->entries[i].key);

    Environment *enclosing = env->enclosing;
    free(env);

    // decrement parent too
    free_environment(enclosing);
}

void env_define(Environment *env, const char *name, int len, Value value) {
    // update if already exists
    for (int i = 0; i < env->count; i++) {
        if (names_equal(env->entries[i].key, name, len)) {
            env->entries[i].val = value;
            return;
        }
    }
    // add new
    char *key = malloc(len + 1);
    memcpy(key, name, len);
    key[len] = '\0';
    env->entries[env->count].key = key;
    env->entries[env->count].val = value;
    env->count++;
}

Value env_get(Environment *env, Token name) {
    for (int i = 0; i < env->count; i++) {
        if (names_equal(env->entries[i].key, name.start, name.length))
            return env->entries[i].val;
    }
    if (env->enclosing)
        return env_get(env->enclosing, name);

    fprintf(stderr, "[line %d] Undefined variable '%.*s'.\n",
            name.line, name.length, name.start);
    exit(70);
}

void env_assign(Environment *env, Token name, Value value) {
    for (int i = 0; i < env->count; i++) {
        if (names_equal(env->entries[i].key, name.start, name.length)) {
            env->entries[i].val = value;
            return;
        }
    }
    if (env->enclosing) {
        env_assign(env->enclosing, name, value);
        return;
    }
    fprintf(stderr, "[line %d] Undefined variable '%.*s'.\n",
            name.line, name.length, name.start);
    exit(70);
}
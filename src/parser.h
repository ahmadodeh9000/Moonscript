#ifndef parser_h
#define parser_h

#include "stmt.h"


typedef struct {
    Token* tokens;
    int count;
    int current;
    int had_error;

} Parser;

void init_parser(Parser* parser, Token* tokens, int count);
Stmt** parse(Parser* parser, int* count);





#endif
#ifndef scanner_h
#define scanner_h

#include "tokens.h"

typedef struct {
    const char* start;
    const char* curr;
    int line;

} Scanner;

void init_scanner(Scanner* scanner, const char* source);


Token scan_token(Scanner* scanner);
Token next_token(Scanner* scanner);

const char* token_type_to_string(TokenType type);


#endif
#include "scanner.h"
#include "common.h"

/* to define the scanner */
void init_scanner(Scanner* scanner, const char* source) {

    if (NULL == scanner) {
        fprintf(stderr,"ERROR : Scanner is null\n");
        exit(67);
    }

    scanner->start  = source;
    scanner->curr   = source;
    scanner->line   = 1;

}
/*================================================================================*/


/* Helper functions */

static bool is_at_end(Scanner* scanner) {
    return *scanner->curr == '\0';
}

static char advance(Scanner* scanner) {
    return *scanner->curr++;
}

static char peek(Scanner* scanner) {
    return *scanner->curr;
}

static char peek_next(Scanner* scanner) {

    if (is_at_end(scanner)) {
        return '\0';
    }

    return scanner->curr[1];

}

static bool match(Scanner* scanner, char exp) {
    if (is_at_end(scanner))     return false;

    if (*scanner->curr != exp)  return false;

    scanner->curr++;

    return true;
 
}

static void skip_whitespace(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(scanner);
                break;
            case '\n':
                scanner->line++;
                advance(scanner);
                break;
            case '/':
                if (peek_next(scanner) == '/') {
                    while (peek(scanner) != '\n' && !is_at_end(scanner))
                        advance(scanner);
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static Token make_token(Scanner* scanner,TokenType type) {

    if (NULL == scanner) {
        fprintf(stderr,"Scanner is null\n");
        exit(67);
    }

    Token t;
    t.type   = type;
    t.start  = scanner->start;
    t.length = (int)(scanner->curr - scanner->start);
    t.line   = scanner->line;
    t.num_value = (t.type == TOKEN_NUMBER) ? strtod(scanner->start,NULL) : 0;

    return t;
}

static Token error_token(Scanner* scanner,const char* message) {

    if (NULL == scanner) {
        fprintf(stderr,"Scanner is null\n");
        exit(67);
    }

    Token t;
    t.type   = TOKEN_ERROR;
    t.start  = message;
    t.length = (int)strlen(message);
    t.line   = scanner->line;
    return t;
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static bool is_alpha(char c) {
    return  (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '_');
}

static TokenType check_keyword(Scanner* scanner,int start, int len, const char* rest, TokenType type) {
    if (scanner->curr - scanner->start == start + len &&
        memcmp(scanner->start + start, rest, len) == 0) {
        return type;
    }
    return TOKEN_IDENT;
}

static TokenType identifier_type(Scanner* scanner) {
    switch (scanner->start[0]) {
        case 'a': return check_keyword(scanner,1, 2, "nd",    TOKEN_AND);
        case 'e': return check_keyword(scanner,1, 3, "lse",   TOKEN_ELSE);
        case 'f':
            if (scanner->curr - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 'a': return check_keyword(scanner,2, 3, "lse", TOKEN_FALSE);
                    case 'o': return check_keyword(scanner,2, 1, "r",   TOKEN_FOR);
                    case 'n': return check_keyword(scanner,2, 0, "",   TOKEN_FUN);
                }
            }
            break;
        case 'i': return check_keyword(scanner,1, 1, "f",        TOKEN_IF);
        case 'n': return check_keyword(scanner,1, 2, "il",    TOKEN_NIL);
        case 'o': return check_keyword(scanner,1, 1, "r",     TOKEN_OR);
        case 'p': return check_keyword(scanner,1, 4, "rint",    TOKEN_PRINT);
        case 'r': return check_keyword(scanner,1, 2, "et",      TOKEN_RET);
        case 't': return check_keyword(scanner,1, 3, "rue",     TOKEN_TRUE);
        case 'l': return check_keyword(scanner,1, 2, "et",      TOKEN_LET);
        case 'w': return check_keyword(scanner,1, 4, "hile",    TOKEN_WHILE);
    }
    return TOKEN_IDENT;
}


static Token identifier(Scanner* scanner) {
    while (is_alpha(peek(scanner)) || is_digit(peek(scanner))) advance(scanner);
    return make_token(scanner,identifier_type(scanner));
}


static Token string(Scanner* scanner) {
    while (peek(scanner) != '"' && !is_at_end(scanner)) {
        if (peek(scanner) == '\n') scanner->line++;
        advance(scanner);
    }

    if (is_at_end(scanner)) return error_token(scanner, "Unterminated string.");

    advance(scanner); // closing "

    // fix: strip the quotes
    Token t;
    t.type      = TOKEN_STRING;
    t.start     = scanner->start + 1;        // skip opening "
    t.length    = (int)(scanner->curr - scanner->start) - 2;  // exclude both "
    t.line      = scanner->line;
    t.num_value = 0;

    //fprintf(stderr, "STRING token: start='%c' length=%d value='%.*s'\n",
    //        *t.start, t.length, t.length, t.start);

    return t;
}



static Token number(Scanner* scanner) {

    while (is_digit(peek(scanner)) && !is_at_end(scanner)) {
        advance(scanner);
    }

    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
        advance(scanner);  // consume the '.'
        while (is_digit(peek(scanner))) advance(scanner);
    }

    return make_token(scanner,TOKEN_NUMBER);

}
/*================================================================================*/

/* for debugging */

/* outputs the token names , it was used to test the scanner */
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_LPAREN:    return "LPAREN";
        case TOKEN_RPAREN:    return "RPAREN";
        case TOKEN_LBRACE:    return "LBRACE";
        case TOKEN_RBRACE:    return "RBRACE";
        case TOKEN_COMMA:     return "COMMA";
        case TOKEN_DOT:       return "DOT";
        case TOKEN_MINUS:     return "MINUS";
        case TOKEN_PLUS:      return "PLUS";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_SLASH:     return "SLASH";
        case TOKEN_STAR:      return "STAR";
        case TOKEN_PERCENT:       return "MOD";  


        /* bitwise operators */
        case TOKEN_LOGICAL_AND: return "LOGICAL AND";
        case TOKEN_LOGICAL_OR:  return "LOGICAL OR";
        case TOKEN_LOGICAL_XOR: return "LOGICAL XOR"; 

        case TOKEN_BANG:      return "BANG";
        case TOKEN_NEQ:       return "NEQ";
        case TOKEN_ASSIGN:    return "ASSIGN";
        case TOKEN_EQ:        return "EQ";
        case TOKEN_GT:        return "GT";
        case TOKEN_GTE:       return "GTE";
        case TOKEN_LT:        return "LT";
        case TOKEN_LTE:       return "LTE";

        case TOKEN_IDENT:     return "IDENTI";
        case TOKEN_STRING:    return "STRING";
        case TOKEN_NUMBER:    return "NUMBER";
        case TOKEN_NIL:       return "NIL";


        case TOKEN_LET:     return "LET";
        case TOKEN_IF:      return "IF";
        case TOKEN_ELSE:    return "ELSE";
        case TOKEN_WHILE:   return "WHILE";
        case TOKEN_TRUE:    return "TRUE";
        case TOKEN_FALSE:   return "FALSE";
        case TOKEN_PRINT:   return "PRINT";
        case TOKEN_RET:     return "RET";
        case TOKEN_FOR:     return "FOR";
        case TOKEN_FUN:     return "FUN";

        case TOKEN_AND:     return "AND";
        case TOKEN_OR:      return "OR";

        case TOKEN_ERROR:   return "ERROR";
        case TOKEN_EOF:     return "EOF";

        default:            return "UNKNOWN";
    }
}
/*================================================================================*/



/* Scanning tokens */

Token scan_token(Scanner* scanner) {

    char c = advance(scanner);

    switch (c) {
        case '(': return make_token(scanner,TOKEN_LPAREN);
        case ')': return make_token(scanner,TOKEN_RPAREN);
        case '{': return make_token(scanner,TOKEN_LBRACE);
        case '}': return make_token(scanner,TOKEN_RBRACE);
        case ',': return make_token(scanner,TOKEN_COMMA);
        case '.': return make_token(scanner,TOKEN_DOT);
        case '+': return make_token(scanner,TOKEN_PLUS);
        case '-': return make_token(scanner,TOKEN_MINUS);
        case '*': return make_token(scanner,TOKEN_STAR);
        case ';': return make_token(scanner,TOKEN_SEMICOLON);
        case '&': return make_token(scanner,TOKEN_LOGICAL_AND);
        case '|': return make_token(scanner,TOKEN_LOGICAL_OR);
        case '^': return make_token(scanner,TOKEN_LOGICAL_XOR);
        case '%': return make_token(scanner,TOKEN_PERCENT);
        case '#': return make_token(scanner,TOKEN_HASHTAG);
        
        case '!': return make_token(scanner,match(scanner,'=') ? TOKEN_NEQ    : TOKEN_BANG);
        case '=': return make_token(scanner,match(scanner,'=') ? TOKEN_EQ   : TOKEN_ASSIGN);
        case '<': return make_token(scanner,match(scanner,'=') ? TOKEN_LTE    : TOKEN_LT);
        case '>': return make_token(scanner,match(scanner,'=') ? TOKEN_GTE : TOKEN_GT);


        case '/':
            if (match(scanner,'/')) {

            // A comment: eat everything until end of line

            while (peek(scanner) != '\n' && !is_at_end(scanner))
                advance(scanner);
            } else {
                return make_token(scanner,TOKEN_SLASH);
            }
            
            break;
        

        case '"': return string(scanner);


        default:
            if (is_digit(c)) return number(scanner);
            if (is_alpha(c)) return identifier(scanner);
            return error_token(scanner,"Unexpected character.");


    }

    return error_token(scanner,"Unexpected character.");

}


Token next_token(Scanner* scanner) {

    skip_whitespace(scanner);

    scanner->start = scanner->curr;

    if (is_at_end(scanner)) return make_token(scanner,TOKEN_EOF);

    return scan_token(scanner);


}

/*================================================================================*/

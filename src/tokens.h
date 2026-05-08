#ifndef tokens_h
#define tokens_h

typedef enum {
    /* literals */
    TOKEN_NUMBER, TOKEN_STRING, TOKEN_IDENT, TOKEN_NIL,

    /* keywords */
    TOKEN_LET, TOKEN_FN, TOKEN_RET, TOKEN_PRINT,
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_TRUE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_AND, TOKEN_OR,

    /* single-char */
    TOKEN_LPAREN, TOKEN_RPAREN,                             /* ( ) */
    TOKEN_LBRACE, TOKEN_RBRACE,                             /* { } */
    TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_DOT,                /* ; ,*/
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,       /* + - * / */
    TOKEN_ASSIGN,                                           /* = */
    TOKEN_BANG,                                             /* ! */
    TOKEN_PERCENT, TOKEN_HASHTAG,                           /* % # */
    TOKEN_LOGICAL_AND,TOKEN_LOGICAL_OR,TOKEN_LOGICAL_XOR,   /* & | ^*/

    /* two-char */
    TOKEN_EQ, TOKEN_NEQ,                                    /* == != */
    TOKEN_LT, TOKEN_GT,                                     /* <  >  */
    TOKEN_LTE, TOKEN_GTE,                                   /* <= >= */

    TOKEN_ERROR,

    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType   type;
    const char* start;
    int         length;
    int         line;
    double      num_value;  /* handles TOKEN_NUMBER*/

} Token;




#endif


#include "common.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"


/* defining the run functions */
static void run(const char* script,Interpreter* interp);
static void run_on_shell(const char* script);


/*=====RUN ON SHELL==============================*/


 
/**/
static void run(const char* script,Interpreter* interp) {
    Scanner scanner;
    init_scanner(&scanner,script);


    int capacity = 64, count =0;

    Token* tokens = malloc(sizeof(Token) * capacity);

    for (;;) {

        if (count >= capacity) {
            capacity *= 2;

            tokens = realloc(tokens, capacity * sizeof(Token));
        }

        Token token = next_token(&scanner);
        tokens[count++] = token;

#ifdef debug__
        printf("type=%s, lexeme='%.*s'\n",
               token_type_to_string(token.type),
               token.length,
               token.start);
#endif
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) break;

    }

    Parser parser;
    init_parser(&parser, tokens, count);
    int stmt_count = 0;
    Stmt **stmts = parse(&parser, &stmt_count);

   


    // print tree
    if (!parser.had_error) {
        interpret(interp,stmts,stmt_count);
    }

    for (int i = 0; i < stmt_count; i++)
        clear_stmt(stmts[i]);
    free(stmts);
    free(tokens);

}

static void run_on_shell(const char* script) {

    Interpreter* interp = init_interpreter();

    char buffer[256];

    for (;;) {
        printf("-> ");

        if (NULL == fgets(buffer,sizeof(buffer),stdin)) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0'; /* removes the newline */

        if (0 == strcmp(buffer,"exit")) {
            printf("bye !\n");
            break;
        }

        run(buffer,interp);

    }

}


static void run_file(const char* path) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        fprintf(stderr, "Could not open file '%s'\n", path);
        exit(74);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);

    if (!buffer) {
        fprintf(stderr, "Not enough memory.\n");
        fclose(file);
        exit(74);
    }

    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    Interpreter* interp = init_interpreter();
    run(buffer, interp);

    free(buffer);
}


int main(int argc, char** argv) {

    if (argc > 2) {
        fprintf(stderr, "Usage: moon [script]\n");
        return 67;
    } else if (argc == 2) {
        run_file(argv[1]);
    }
    else {
        run_on_shell(argv[0]);
    }

    return 0;

}


/*
 * Parser
 */
#include <stdio.h>
#include <stdlib.h>

enum SYMBOL {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    INT
};

typedef struct token_struct {
    SYMBOL symbol = -1;
    int lexeme;
} token;

token
get_token(char* line)
{
    token t;
    int c = line[0];

    switch (c) {
        case '+': t = {PLUS}; break;
        case '-': t = {MINUS}; break;
        case '*': t = {MULTIPLY}; break;
        case '/': t = {DIVIDE}; break;
        default:
            if (isdigit(c)) {
                t = {INT, i};
            }
            break;
    }


    return t;
}

char*
tokenize(char* line)
{
    // for whole line
    // divide by tokens alpha & (+-*/)
    // send each piece to get_token
    //
    if ( line[0] == '+' ) return PLUS;
    if ( strncmp(line, "quit", 4) == 0 || strncmp(line, "exit", 4) == 0) {
        //printf("\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("Unrecognized %s", line);
    }

    return line;
}

void
eval(char* ast)
{
    printf("%s", ast);
}


int
main(int argc, char argv[])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *ast = NULL;

    fp = fdopen(0, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("> ");
    while ((read = getline(&line, &len, fp)) != -1) {
        ast = tokenize(line);
        eval(ast);
        printf("> ");
    }

    free(line);
    exit(EXIT_SUCCESS);
}

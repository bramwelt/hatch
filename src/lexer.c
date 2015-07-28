/*
 * Hatch
 * (c) Trevor Bramwell - GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#include <lexer.h>

ParseFile*
newFile(char* filename)
{

    ParseFile* parsefile = malloc(sizeof(ParseFile));
    parsefile->filename = filename;
    parsefile->fp = fopen(filename, "r");
    parsefile->line = 1;
    parsefile->column = 0;

    if (parsefile->fp == NULL) {
        exit(EXIT_FAILURE);
    }

    return parsefile;
}

void
removeFile(ParseFile* parsefile) {
    fclose(parsefile->fp);
    free(parsefile);
}

void
emit(Token* token) {
    switch(token->symbol) {
        case ADD: printf("<ADD, '+'>"); break;
        case SUBTRACT: printf("<SUBTRACT, '-'>"); break;
        case MULTIPLY: printf("<MULTIPLY, '*'>"); break;
        case DIVIDE: printf("<DIVIDE, '/'>"); break;
        case INT: printf("<INT, '%d'>", token->lexeme); break;
        default: break;
    }
}

void
get_token(ParseFile *file, Token *token)
{
    int c = getc(file->fp);

    while (isblank(c) || isalpha(c) || c == '\n') {
        ++(file->column);
        if (c == '\n') {
            ++(file->line);
            file->column = 0;
        }
        c = getc(file->fp);
    }

    if (isdigit(c)) {
        token->symbol = INT;
        token->lexeme = (c - '0');
        while (isdigit(c = getc(file->fp))) {
            token->lexeme = token->lexeme * 10 + (c - '0');
        }
        ungetc(c, file->fp);
        // Update the column to the end of the number
        file->column = file->column + floor(log10(abs(token->lexeme))) + 1;
    } else {
        switch (c) {
            case '+':
                token->symbol = ADD;
                break;

            case '-':
                token->symbol = SUBTRACT;
                break;

            case '*':
                token->symbol = MULTIPLY;
                break;

            case '/':
                token->symbol = DIVIDE;
                break;

            case EOF:
                token->symbol = END;
                break;

            default:
                token->symbol = UNKNOWN;
                printf("%s %d:%d unrecognized symbol '%c'\n",file->filename, file->line, file->column, c);
        }

        ++(file->column);
    }
}

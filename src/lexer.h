/*
 * Hatch
 * (c) Trevor Bramwell - GPLv3
 *
 */
#ifndef _HATCH_LEXER_H_
#define _HATCH_LEXER_H_

typedef enum Symbol {
    ADD = 0,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    INT,
    END,
    UNKNOWN
} Symbol;

typedef struct Token {
    Symbol symbol;
    int lexeme;
} Token;

typedef struct ParseFile {
    char* filename;
    FILE* fp;
    int line;
    int column;
} ParseFile;

void get_token(ParseFile* file, Token *token);

ParseFile* newFile(char *filename);
void removeFile(ParseFile* parsefile);

void emit(Token* token);

#endif

/*
 * Hatch
 * (c) Trevor Bramwell - GPLv3
 *
 * A Simple Calculator Parser
 *
 *      Formal Grammar
 * -------------------------
 * Expr        = MultDivExpr
 *             | AddSubExpr
 *
 * AddSubExpr  = + IExpr IExpr
 *             | - IExpr IExpr
 *
 * MultDivExpr = * IExpr IExpr
 *             | / IExpr IExpr
 *
 * IExpr       = Expr
 *             | Integer
 *
 * Integer     = [0-9]+
 * -------------------------
 *
 * Here is an example in infix notation:
 *
 *   (2 * 8) + (15 / 5) - 4
 *
 * [Note: Parenthesis added for clarification, not part of grammar]
 *
 * As this grammer uses prefix notation, this example would be written:
 *
 *   - + * 2 8 / 15 5 4
 *
 * Which evaluates to 15.
 *
 * Quiting the parser can be done with Ctrl-D or Ctrl-C.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include <lexer.h>

typedef struct ParseTree {
    Token token;
    int result;
} ParseTree;

int IExpr(Token *token, ParseFile *parsefile);
int Expr(Token *token, ParseFile *parsefile);
int AddSubExpr(Token *token, ParseFile *parsefile);
int MultDivExpr(Token *token, ParseFile *parsefile);
void match(Symbol symbol, Token *token, ParseFile *parsefile);

int
Expr(Token* token, ParseFile *parsefile) {
    if (token->symbol == MULTIPLY || token->symbol == DIVIDE) {
        return MultDivExpr(token, parsefile);
    } else if (token->symbol == ADD || token->symbol == SUBTRACT) {
        return AddSubExpr(token, parsefile);
    } else {
        printf("Error in Expr.\n");
        exit(EXIT_FAILURE);
    }
}

int
IExpr(Token* token, ParseFile *parsefile) {
    if (token->symbol >= ADD && token->symbol <= DIVIDE) {
        return Expr(token, parsefile);
    } else if (token->symbol == INT) {
        int a = token->lexeme;
        match(INT, token, parsefile);
        return a;
    } else {
        printf("Error in IExpr.\n");
        exit(EXIT_FAILURE);
    }
}

int
AddSubExpr(Token* token, ParseFile *parsefile) {
    int a, b, c;
    if (token->symbol == ADD ) {
        match(ADD, token, parsefile);
        a = IExpr(token, parsefile);
        b = IExpr(token, parsefile);
        c = a + b;
        return c;
    } else if (token->symbol == SUBTRACT) {
        match(SUBTRACT, token, parsefile);
        a = IExpr(token, parsefile);
        b = IExpr(token, parsefile);
        c = a - b;
        return c;
    } else {
        printf("Error in AddSub\n");
        exit(EXIT_FAILURE);
    }
}

int
MultDivExpr(Token* token, ParseFile *parsefile) {
    int a, b, c;
    if (token->symbol == MULTIPLY) {
        match(MULTIPLY, token, parsefile);
        a = IExpr(token, parsefile);
        b = IExpr(token, parsefile);
        c = a * b;
        return c;
    } else if (token->symbol == DIVIDE) {
        match(DIVIDE, token, parsefile);
        a = IExpr(token, parsefile);
        b = IExpr(token, parsefile);
        c = a / b;
        return c;
    } else {
        printf("Error in MultDiv\n");
        exit(EXIT_FAILURE);
    }
}

void
match(Symbol symbol, Token* token, ParseFile* parsefile) {
    if (symbol == token->symbol) {
        // Enable to see tokens as they are matched.
        //emit(token);
        get_token(parsefile, token);
    } else {
        printf("%s:%d - Unknown symbol %d\n", parsefile->filename, parsefile->line, symbol);
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char **argv)
{
    if (argc < 1) {
        printf("Please provide a file to parse.\n");
        exit(1);
    }

    ParseFile *parsefile = newFile(argv[1]);

    printf("Compiling %s\n", parsefile->filename);

    ParseTree pt;
    pt.result = 0;

    get_token(parsefile, &pt.token);

    while (pt.token.symbol != END) {
        //emit(&pt.token);
        //printf("\n");
        //get_token(parsefile, &pt.token);
        pt.result = IExpr(&pt.token, parsefile);
        printf("> %d\n", pt.result);
    }

    printf("Done Compiling: %s\n", parsefile->filename);

    removeFile(parsefile);
    exit(EXIT_SUCCESS);
}

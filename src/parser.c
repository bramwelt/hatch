/*
 * Simple Calculator Parser
 *
 *
 * Expr     = Integer
 *          | MultDivExpr
 *          | AddSubExpr
 *
 * AddSubExpr  = + Expr Expr
 *             | - Expr Expr
 *
 * MultDivExpr = * Expr Expr
 *             | / Expr Expr
 *
 * Integer  = [0-9]+
 *
 * Example:
 *
 *   (2 * 8) + (15 / 5) - 4 ~> 15
 *
 *   [Note: Parenthesis added for clarification, not part of grammar]
 *
 *   Would be written:
 *   - + * 2 8 / 15 5 4
 *
 * TODO: Split out Lexer & Parser
 * TODO: Each Parse construct should return a Tree
 * TODO: A Tree a recursive struct with nodes being Tokens.
 */
#include <stdio.h>
#include <stdlib.h>

typedef enum Symbol {
    ADD = 0,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    INT,
    EOL,
    END
} Symbol;

typedef struct Token {
    Symbol symbol;
    int lexeme;
} Token;


// Globals - :(
FILE* fp;
Token token;

/*
typedef struct Parser {
    FILE* file;
    int line;
    int column;
} Parser;
*/


/*
match(Token) - match the given token and advance if present
expect()     - aka 'peek' - look ahead at the token so we can decide which
               path to take
*/

void Expr();
void AddSubExpr();
void MultDivExpr();
void match(Symbol s);
void emit(Symbol s);

void
Expr() {
    if (token.symbol == INT) {
        match(INT); emit(INT);
    } else if (token.symbol == MULTIPLY || token.symbol == DIVIDE) {
        MultDivExpr();
    } else if (token.symbol == ADD || token.symbol == SUBTRACT) {
        AddSubExpr();
    } else {
        printf("Error in Expr.\n"); exit(EXIT_FAILURE);
    }
}

void
AddSubExpr() {
    if (token.symbol == ADD ) {
        match(ADD); emit(ADD); Expr(); Expr();
    } else if (token.symbol == SUBTRACT) {
        match(SUBTRACT); emit(SUBTRACT); Expr(); Expr();
    } else {
        printf("Error in AddSub\n"); exit(EXIT_FAILURE);
    }
}

void
MultDivExpr() {
    if (token.symbol == MULTIPLY) {
        match(MULTIPLY); emit(MULTIPLY); Expr(); Expr();
    } else if (token.symbol == DIVIDE) {
        match(DIVIDE); emit(DIVIDE); Expr(); Expr();
    } else {
        printf("Erorr in MultDiv\n"); exit(EXIT_FAILURE);
    }
}

Symbol
get_token()
{
    int c = getc(fp);

    while(isalpha(c) || isblank(c)) {
        c = getc(fp);
    }

    switch (c) {
        case '+':
            token.symbol = ADD;
            return ADD;

        case '-':
            token.symbol = SUBTRACT;
            return SUBTRACT;

        case '*':
            token.symbol = MULTIPLY;
            return MULTIPLY;

        case '/':
            token.symbol = DIVIDE;
            return DIVIDE;

        case EOF:
            token.symbol = END;
            return END;

        case '\n':
            token.symbol = EOL;
            return EOL;

        default:
            if (isdigit(c)) {
                token.symbol = INT;
                token.lexeme = (c - '0');
                while (isdigit(c = getc(fp))) {
                    token.lexeme = token.lexeme * 10 + (c - '0');
                }
                ungetc(c, fp);
                return INT;
            } else {
                c = getc(fp);
            }
    }
}

void
emit(Symbol s) {
    switch(s) {
        case ADD: printf("<ADD, '+'>"); break;
        case SUBTRACT: printf("<SUBTRACT, '-'>"); break;
        case MULTIPLY: printf("<MULTIPLY, '*'>"); break;
        case DIVIDE: printf("<DIVIDE, '/'>"); break;
        case INT: printf("<INT, '%d'>", token.lexeme); break;
        default: break;
    }
}

void
match(Symbol s) {
    if (s == token.symbol) {
        get_token();
    } else {
        printf("ERR - Unknown symbol: %d\n", s);
        exit(EXIT_FAILURE);
    }
}


int
main(int argc, char argv[])
{

    fp = fdopen(0, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("> ");

    get_token();
    while (token.symbol != END) {
        emit(token.symbol);
        get_token();
        if (token.symbol == EOL) {
            printf("\n> ");
            get_token();
        }
    }
    printf("\n");

    close(fp);
    exit(EXIT_SUCCESS);
}

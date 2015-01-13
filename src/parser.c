/*
 * Simple Calculator Parser
 * by Trevor Bramwell
 * (c) 2014 - GPLv3
 *
 *      Formal Grammar
 * -------------------------
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
 * -------------------------
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
 * Quiting the parser can be done with ^D or ^C.
 *
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

int Expr();
int AddSubExpr();
int MultDivExpr();
void match(Symbol s);
void emit(Symbol s);

int
Expr() {
    if (token.symbol == INT) {
        int a = token.lexeme;
        match(INT);
        return a;
    } else if (token.symbol == MULTIPLY || token.symbol == DIVIDE) {
        return MultDivExpr();
    } else if (token.symbol == ADD || token.symbol == SUBTRACT) {
        return AddSubExpr();
    } else {
        printf("Error in Expr.\n"); exit(EXIT_FAILURE);
    }
}

int
AddSubExpr() {
    int a, b, c;
    if (token.symbol == ADD ) {
        match(ADD);
        a = Expr();
        b = Expr();
        c = a + b;
        return c;
    } else if (token.symbol == SUBTRACT) {
        match(SUBTRACT);
        a = Expr(); 
        b = Expr();
        c = a - b;
        return c;
    } else {
        printf("Error in AddSub\n"); exit(EXIT_FAILURE);
    }
}

int
MultDivExpr() {
    int a, b, c;
    if (token.symbol == MULTIPLY) {
        match(MULTIPLY);
        a = Expr();
        b = Expr();
        c = a * b;
        return c;
    } else if (token.symbol == DIVIDE) {
        match(DIVIDE);
        a = Expr();
        b = Expr();
        c = a / b;
        return c;
    } else {
        printf("Error in MultDiv\n"); exit(EXIT_FAILURE);
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
        // Enable to see tokens as they are matched.
        // emit(s);
        get_token();
    } else {
        printf("ERR - Unknown symbol: %d\n", s);
        exit(EXIT_FAILURE);
    }
}


int
main(int argc, char argv[])
{
    int result;

    fp = fdopen(0, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("> ");

    get_token();
    while (token.symbol != END) {
        result = Expr();
        if (token.symbol == EOL) {
            // Uncomment this if token debugging is enabled.
            // printf("\n");
            printf("%d", result);
            printf("\n> ");
            get_token();
        }
    }
    printf("\n");

    close(fp);
    exit(EXIT_SUCCESS);
}

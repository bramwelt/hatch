hatch: src/parser.c src/lexer.c
	gcc -g -Wall -Wextra -std=c11 -I ./src src/lexer.c src/parser.c -o hatch -lm

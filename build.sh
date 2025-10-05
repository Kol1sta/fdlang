gcc src/main.c src/lexer.c src/parser.c -o dist/fd -Wall -Wextra
dist/fd ./samples/a.fd

#ifndef FD
#define FD

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYWORD_LEN 8
#define MAX_INT32_VALUE 10

typedef enum {
    LPAREN,
    RPAREN,
    SEMICOLON,

    EXIT,
    IF,

    INT32,
    CHAR,

    UNDEFINED_TOKEN
} token_type_t;

typedef struct {
    token_type_t type;
    char* value;
} token_t;

token_t **tokenize(FILE *file);
void free_tokens(token_t **tokens);
void print_tokens(token_t **tokens);

token_t *generate_separator(token_type_t type);
token_t *generate_number(int *current, FILE *file);
token_t *generate_keyword(int *current, FILE *file);

#endif

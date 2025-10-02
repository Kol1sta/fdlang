#ifndef FD
#define FD

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYWORD_LEN 8

typedef enum {
    LPAREN,
    RPAREN,
    SEMICOLON,

    EXIT,
    IF,

    INT32,
    CHAR
} token_type_t;

typedef struct {
    token_type_t type;
    void* value;
} token_t;

typedef struct {
    token_type_t type;
    int32_t value;
} int_token_t;

void **tokenize(FILE *file);
void free_tokens(void** tokens);
void print_tokens(void** tokens);

token_t *generate_separator(token_type_t type, FILE *file);
int_token_t *generate_number(int *current, FILE *file);
token_t *generate_keyword(int *current, FILE *file);

#endif

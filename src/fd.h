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
} token_t;

typedef struct {
    token_type_t type;
    int32_t value;
} int_token_t;

void* tokenize(FILE *file);
int_token_t generate_number(char *current, FILE* file);
token_t generate_keyword(char *current, FILE *file);

#endif

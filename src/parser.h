#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

typedef struct {

} node_t;

node_t *parse(token_t **tokens);

#endif

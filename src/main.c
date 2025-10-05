#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Need to set filepath (fd <filepath>)");
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    token_t **tokens = tokenize(file);
    print_tokens(tokens);
    printf("%s cur\n", ((token_t*)tokens[2])->value);
    node_t *nodes = parse(tokens);
    free_tokens(tokens);

    return 0;
}

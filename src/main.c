#include "./fd.h"

int main(int argc, char **argv) {
    FILE *file = fopen("./samples/a.fd", "r");
    int symbol;
    if(file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    void **tokens = tokenize(file);
    print_tokens(tokens);
    free_tokens(tokens);

    return 0;
}

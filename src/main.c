#include "./fd.h"

int main(int argc, char **argv) {
    FILE *file = fopen("./samples/a.fd", "r");
    int symbol;
    if(file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    tokenize(file);

    return 0;
}

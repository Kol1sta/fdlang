#include "fd.h"

void* tokenize(FILE *file) {
    char symbol = getc(file);

    while(symbol != EOF) {
        if(symbol == ';') {
            printf("SEMICOLON FINDED\n");
        } else if(isdigit(symbol) || symbol == '-') {
            int_token_t a = generate_number(&symbol, file);
        } else if(isalpha(symbol)) {
            token_t a = generate_keyword(&symbol, file);
        }

        symbol = getc(file);
    }

    fclose(file);
    return NULL;
}

int_token_t generate_number(char *current, FILE *file) {
    uint8_t is_negative = 0;
    uint8_t digits = 0;
    int_token_t token = { .type=INT32, .value=0 };

    if(*current == '-') {
        is_negative = 1;
        *current = getc(file);

        if(!isdigit(*current)) {
            token.type = CHAR;
            token.value = '-';
            return token;
        }
    }

    while(isdigit(*current) && *current != EOF) {
        token.value = token.value * 10 + (int)(*current - '0');
        *current = getc(file);
    }

    if(is_negative) token.value = -token.value;

    return token;
}

token_t generate_keyword(char *current, FILE *file) {
    token_t token;
    char* buffer = (char*)malloc(sizeof(char) * MAX_KEYWORD_LEN + 1);
    int index = 0;

    while(index < MAX_KEYWORD_LEN && isalpha(*current) && *current != EOF) {
        buffer[index] = *current;
        index++;
        *current = getc(file);
    }

    buffer[index] = '\0';
    printf("%s\n", buffer);

    switch(buffer[0]) {
        case 'i':
            if(strcmp(buffer, "if") == 0) token.type = IF;
            break;
        case 'e':
            if(strcmp(buffer, "exit") == 0) token.type = EXIT;
            break;
        default:
            // undefined token
            break;
    }

    free(buffer);
    return token;
}

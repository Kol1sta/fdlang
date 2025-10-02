#include "fd.h"

void* tokenize(FILE *file) {
    char symbol = getc(file);
    void **tokens = (void*)malloc(sizeof(token_t));
    void *current_token;
    size_t index = 0;

    while(symbol != EOF) {
        if(symbol == ';') {
            current_token = generate_separator(SEMICOLON, file);
        } else if(symbol == '(') {
            current_token = generate_separator(LPAREN, file);
        } else if(symbol == ')') {
            current_token = generate_separator(RPAREN, file);
        } else if(isdigit(symbol) || symbol == '-') {
            current_token = generate_number(&symbol, file);
        } else if(isalpha(symbol)) {
            current_token = generate_keyword(&symbol, file);
        }

        symbol = getc(file);
        index++;
    }

    fclose(file);
    return NULL;
}

void print_tokens(void* tokens) {

}

token_t *generate_separator(token_type_t type, FILE *file) {
    token_t *token = (token_t*)malloc(sizeof(token_t));
    if(token == NULL) {
        fprintf(stderr, "Failed to allocate memory for token separator");
        return NULL;
    }

    token->type = type;

    return token;
}

int_token_t *generate_number(char *current, FILE *file) {
    uint8_t is_negative = 0;
    uint8_t digits = 0;
    int_token_t *token = (int_token_t*)malloc(sizeof(int_token_t));

    if(token == NULL) {
        fprintf(stderr, "Failed to allocate memory for token number");
        return NULL;
    }

    if(*current == '-') {
        is_negative = 1;
        digits = 1;
        *current = getc(file);

        if(!isdigit(*current)) {
            token->type = CHAR;
            token->value = '-';
            ungetc(*current, file);
            return token;
        }
    }

    token->type = INT32;
    token->value = 0;
    while(isdigit(*current) && *current != EOF) {
        token->value = token->value * 10 + (int)(*current - '0');
        *current = getc(file);
    }

    if((digits > 1 && is_negative) || (digits && !is_negative)) {
        ungetc(*current, file);
    }

    if(is_negative) token->value = -token->value;

    return token;
}

token_t *generate_keyword(char *current, FILE *file) {
    token_t* token = (token_t*)malloc(sizeof(token_t));
    char *buffer = (char*)malloc(sizeof(char) * MAX_KEYWORD_LEN + 1);
    int index = 0;

    if(token == NULL || buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for token number or buffer");
        return NULL;
    }

    while(index < MAX_KEYWORD_LEN && isalpha(*current) && *current != EOF) {
        buffer[index] = *current;
        index++;
        *current = getc(file);
    }

    ungetc(*current, file);

    buffer[index] = '\0';

    switch(buffer[0]) {
        case 'i':
            if(strcmp(buffer, "if") == 0) token->type = IF;
            break;
        case 'e':
            if(strcmp(buffer, "exit") == 0) token->type = EXIT;
            break;
        default:
            // undefined token
            break;
    }

    free(buffer);
    return token;
}

#include "fd.h"
#include <stdio.h>

void **tokenize(FILE *file) {
    if(file == NULL) {
        fprintf(stderr, "Failed to read file in lexer");
        return NULL;
    }

    int symbol;
    void **tokens = NULL;
    void *current_token;

    size_t capacity = 0;
    size_t index = 0;

    while((symbol = getc(file)) != EOF) {
        if(isspace(symbol)) continue;

        current_token = NULL;

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
        } else {
            fprintf(stderr, "Undefined symbol");
        }

        if(current_token) {
            if(index >= capacity) {
                capacity = capacity == 0 ? 16 : capacity * 2;
                void **new_tokens = (void**)realloc(tokens, sizeof(void*) * capacity);

                if(new_tokens == NULL) {
                    fprintf(stderr, "Failed to reallocate memory for tokens");
                    free_tokens(tokens);
                    return NULL;
                }

                tokens = new_tokens;
            }

            tokens[index] = current_token;
            index++;
        }
    }

    if(tokens) {
        void **new_tokens = (void**)realloc(tokens, sizeof(void*) * (index + 1));
        if(new_tokens) {
            tokens = new_tokens;
            tokens[index] = NULL;
        }
    }

    fclose(file);
    return tokens;
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

int_token_t *generate_number(int *current, FILE *file) {
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

    if(*current != EOF) {
        ungetc(*current, file);
    }

    if(is_negative) {
        token->value = -token->value;
    }

    return token;
}

token_t *generate_keyword(int *current, FILE *file) {
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

void free_tokens(void **tokens) {
    if(tokens) {
        for(size_t i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }

        free(tokens);
    }
}

void print_tokens(void **tokens) {
    if(tokens == NULL) {
        fprintf(stderr, "No tokens for print");
        return;
    }

    printf("Tokens:\n");
    printf("Index | Type                | Value\n");
    printf("------|---------------------|-------\n");

    for(size_t i = 0; tokens[i] != NULL; i++) {
        token_t *base_token = (token_t*)tokens[i];

        printf("%5zu | ", i);

        switch(base_token->type) {
            case SEMICOLON:
                printf("SEMICOLON           | ;\n");
                break;
            case LPAREN:
                printf("LPAREN              | (\n");
                break;
            case RPAREN:
                printf("RPAREN              | )\n");
                break;
            case IF:
                printf("IF                  | if\n");
                break;
            case EXIT:
                printf("EXIT                | exit\n");
                break;
            case CHAR:
                printf("CHAR                | '%c'\n", ((int_token_t*)base_token)->value);
                break;
            case INT32:
                printf("INT32               | %d\n", ((int_token_t*)base_token)->value);
                break;
            default:
                printf("UNKNOWN             | ?\n");
                break;
        }
    }
}

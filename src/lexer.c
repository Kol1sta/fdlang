#include "lexer.h"

token_t **tokenize(FILE *file) {
    if(file == NULL) {
        fprintf(stderr, "Failed to read file in lexer");
        return NULL;
    }

    int symbol;
    token_t **tokens = NULL;
    token_t *current_token;

    size_t capacity = 0;
    size_t index = 0;

    while((symbol = getc(file)) != EOF) {
        if(isspace(symbol)) continue;

        current_token = NULL;

        if(symbol == ';') {
            current_token = generate_separator(SEMICOLON);
        } else if(symbol == '(') {
            current_token = generate_separator(LPAREN);
        } else if(symbol == ')') {
            current_token = generate_separator(RPAREN);
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
                token_t **new_tokens = (token_t**)realloc(tokens, sizeof(token_t*) * capacity);

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
        token_t **new_tokens = (token_t**)realloc(tokens, sizeof(token_t*) * (index + 1));
        if(new_tokens) {
            tokens = new_tokens;
            tokens[index] = NULL;
        }
    }

    fclose(file);
    return tokens;
}

token_t *generate_separator(token_type_t type) {
    token_t *token = (token_t*)malloc(sizeof(token_t));

    if(token == NULL) {
        fprintf(stderr, "Failed to allocate memory for token separator");
        return NULL;
    }

    token->type = type;
    token->value = NULL;

    return token;
}

token_t *generate_number(int *current, FILE *file) {
    uint8_t is_negative = 0;
    token_t *token = (token_t*)malloc(sizeof(token_t));
    short index = 0;

    if(token == NULL) {
        fprintf(stderr, "Failed to allocate memory for token number");
        return NULL;
    }

    token->type = INT32;
    token->value = (char*)malloc(sizeof(char) * MAX_INT32_VALUE + 1);
    token->value[MAX_INT32_VALUE] = '\0';

    if(*current == '-') {
        is_negative = 1;
        *current = getc(file);

        if(!isdigit(*current)) {
            token->type = CHAR;
            token->value = "-";
            ungetc(*current, file);
            return token;
        }

        token->value[0] = '-';
        token->value[1] = '\0';
        index++;
    }

    while(isdigit(*current) && *current != EOF && index < MAX_INT32_VALUE + 1) {
        token->value[index] = *current;
        token->value[index + 1] = '\0';
        *current = getc(file);
        index++;
    }

    if(*current != EOF) {
        ungetc(*current, file);
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
            token->type = UNDEFINED_TOKEN;
            break;
    }

    free(buffer);
    return token;
}

void free_tokens(token_t **tokens) {
    if(tokens) {
        for(size_t i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }

        free(tokens);
    }
}

void print_tokens(token_t **tokens) {
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
                printf("CHAR                | '%s'\n", ((token_t*)base_token)->value);
                break;
            case INT32:
                printf("INT32               | %s\n", ((token_t*)base_token)->value);
                break;
            default:
                printf("UNKNOWN             | ?\n");
                break;
        }
    }
}

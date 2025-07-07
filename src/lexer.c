#include "lexer.h"

#include <stdlib.h>
#include <string.h>

token_t *text_to_token(char *text) {
    token_t *token = malloc(sizeof(token_t));
    token->token_type = TOKEN_STRING;
    token->value = text;
    token->length = strlen(text);
    token->next_token = NULL;
    return token;
}

void free_token(token_t *token) {
    free(token->value);
    free(token);
}

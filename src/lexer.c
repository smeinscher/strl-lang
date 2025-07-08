#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_type_t get_token_type(char *text) {
    if (strcmp(text, "set") == 0) {
        return TOKEN_SET;
    } else if (strcmp(text, "while") == 0) {
        return TOKEN_WHILE;
    } else if (strcmp(text, "endwhile") == 0) {
        return TOKEN_ENDWHILE;
    } else if (strcmp(text, "if") == 0) {
        return TOKEN_IF;
    } else if (strcmp(text, "endif") == 0) {
        return TOKEN_ENDIF;
    } else if (strcmp(text, "print") == 0) {
        return TOKEN_PRINT;
    } else if (strcmp(text, "=") == 0) {
        return TOKEN_SET_EQUAL;
    } else if (strcmp(text, "+") == 0) {
        return TOKEN_PLUS;
    } else if (strcmp(text, "-") == 0) {
        return TOKEN_MINUS;
    } else if (strcmp(text, "*") == 0) {
        return TOKEN_MULTIPLY;
    } else if (strcmp(text, "/") == 0) {
        return TOKEN_DIVIDE;
    } else if (strcmp(text, "==") == 0) {
        return TOKEN_EQUAL;
    } else if (strcmp(text, "!=") == 0) {
        return TOKEN_NOT_EQUAL;
    } else if (strcmp(text, "<") == 0) {
        return TOKEN_LESS_THAN;
    } else if (strcmp(text, "<=") == 0) {
        return TOKEN_LESS_THAN_EQUAL_TO;
    } else if (strcmp(text, ">") == 0) {
        return TOKEN_GREATER_THAN;
    } else if (strcmp(text, ">=") == 0) {
        return TOKEN_GREATER_THAN_EQUAL_TO;
    } else if (strcmp(text, "true") == 0) {
        return TOKEN_TRUE;
    } else if (strcmp(text, "false") == 0) {
        return TOKEN_FALSE;
    } else if (text[0] == '"' && text[strlen(text) - 1] == '"') {
        return TOKEN_STRING;
    } else {
        bool valid_int = true;
        for (int i = 0; i < strlen(text); i++) {
            if (text[0] - '0' > 9) {
                valid_int = false;
                break;
            }
        }
        if (valid_int) {
            return TOKEN_INT;
        }
    }
    return TOKEN_VARIABLE;
}

token_t *text_to_token(char *text) {
    token_t *token = malloc(sizeof(token_t));
    token->token_type = get_token_type(text);
    token->value = text;
    token->length = strlen(text);
    token->next_token = NULL;
    return token;
}

void free_token(token_t *token) {
    free(token->value);
    free(token);
}

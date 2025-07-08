#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_INT = 0,
    TOKEN_STRING = 1,
    TOKEN_TRUE = 2,
    TOKEN_FALSE = 3,
    TOKEN_VARIABLE = 4,
    // Keywords
    TOKEN_SET = 10,

    // Labels
    TOKEN_WHILE = 21,
    TOKEN_ENDWHILE = 22,
    TOKEN_IF = 23,
    TOKEN_ENDIF = 24,
    TOKEN_PRINT = 25,

    // Operators
    TOKEN_SET_EQUAL = 30,
    TOKEN_PLUS = 31,
    TOKEN_MINUS = 32,
    TOKEN_MULTIPLY = 33,
    TOKEN_DIVIDE = 34,
    // Logical Operators
    TOKEN_EQUAL = 40,
    TOKEN_NOT_EQUAL = 41,
    TOKEN_LESS_THAN = 42,
    TOKEN_LESS_THAN_EQUAL_TO = 43,
    TOKEN_GREATER_THAN = 44,
    TOKEN_GREATER_THAN_EQUAL_TO = 45
} token_type_t;

typedef struct token token_t;
typedef struct token {
    token_type_t token_type;
    char *value;
    int length;
    token_t *next_token;
} token_t;

token_t *text_to_token(char *text);

void free_token(token_t *token);

#endif  // LEXER_H

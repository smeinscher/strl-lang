#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_EOF = -1,
    TOKEN_NEWLINE = 0,
    TOKEN_INT = 1,
    TOKEN_STRING = 2,
    TOKEN_TRUE = 3,
    TOKEN_FALSE = 4,

    // Keywords
    TOKEN_SET = 10,

    // Labels
    TOKEN_WHILE = 21,
    TOKEN_IF = 22,

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

#endif // LEXER_H

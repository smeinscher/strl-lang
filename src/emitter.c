#include "lexer.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emitter.h"
#include "parser.h"

#define VAR_TABLE_SIZE 1024

// djb2
// https://stackoverflow.com/questions/7666509/hash-function-for-string
static size_t hash(const char *variable_name) {
    unsigned long hash = 5381;
    int c;
    while ((c = *variable_name++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % VAR_TABLE_SIZE;
}

static char *execute_recursive(ast_node_t *node, char **var_table) {
    switch (node->kind) {
        case OPERATOR:
            // char *vals[2];
            // vals[0] = execute_recursive(node->children[0], var_table);
            // vals[1] = execute_recursive(node->children[1], var_table);
            switch (node->token->token_type) {
                case TOKEN_SET_EQUAL: {
                    token_t *token1 = node->children[0]->token;
                    token_t *token2 = node->children[1]->token;
                    if (token1->token_type != TOKEN_VARIABLE) {
                        printf("Invalid token type for assignment\n");
                        return NULL;
                    }
                    int token_value;
                    if (token2->token_type == TOKEN_VARIABLE) {
                        size_t token2_hash_val = hash(token2->value);
                        token_value = atoi(var_table[token2_hash_val]);
                    } else {
                        token_value = atoi(token2->value);
                    }
                    size_t token1_hash_val = hash(token1->value);
                    char *buffer = malloc(sizeof(char) * 11);
                    sprintf(buffer, "%d", token_value);
                    var_table[token1_hash_val] = buffer;
                    return var_table[token1_hash_val];
                }
                case TOKEN_PLUS:
                case TOKEN_MINUS:
                case TOKEN_MULTIPLY:
                case TOKEN_DIVIDE: {
                    token_t *token1 = node->children[0]->token;
                    token_t *token2 = node->children[1]->token;
                    int token1_value;
                    if (token1->token_type == TOKEN_VARIABLE) {
                        size_t token1_hash_val = hash(token1->value);
                        token1_value = atoi(var_table[token1_hash_val]);
                    } else {
                        token1_value = atoi(token1->value);
                    }
                    int token2_value;
                    if (token2->token_type == TOKEN_VARIABLE) {
                        size_t token2_hash_val = hash(token2->value);
                        token2_value = atoi(var_table[token2_hash_val]);
                    } else {
                        token2_value = atoi(token2->value);
                    }
                    int result;
                    switch (node->token->token_type) {
                        case TOKEN_PLUS:
                            result = token1_value + token2_value;
                            break;
                        case TOKEN_MINUS:
                            result = token1_value - token2_value;
                            break;
                        case TOKEN_MULTIPLY:
                            result = token1_value * token2_value;
                            break;
                        case TOKEN_DIVIDE:
                            result = token1_value / token2_value;
                            break;
                        default:
                            result = 0;
                            break;
                    }
                    char *buffer = malloc(sizeof(char) * 11);
                    sprintf(buffer, "%d", result);
                    return buffer;
                }
                case TOKEN_EQUAL:
                case TOKEN_NOT_EQUAL:
                case TOKEN_LESS_THAN:
                case TOKEN_LESS_THAN_EQUAL_TO:
                case TOKEN_GREATER_THAN:
                case TOKEN_GREATER_THAN_EQUAL_TO: {
                    token_t *token1 = node->children[0]->token;
                    token_t *token2 = node->children[1]->token;
                    int token1_value;
                    if (token1->token_type == TOKEN_VARIABLE) {
                        size_t token1_hash_val = hash(token1->value);
                        token1_value = atoi(var_table[token1_hash_val]);
                    } else {
                        token1_value = atoi(token1->value);
                    }
                    int token2_value;
                    if (token2->token_type == TOKEN_VARIABLE) {
                        size_t token2_hash_val = hash(token2->value);
                        token2_value = atoi(var_table[token2_hash_val]);
                    } else {
                        token2_value = atoi(token2->value);
                    }
                    bool result;
                    switch (node->token->token_type) {
                        case TOKEN_EQUAL:
                            result = token1_value == token2_value;
                            break;
                        case TOKEN_NOT_EQUAL:
                            result = token1_value != token2_value;
                            break;
                        case TOKEN_LESS_THAN:
                            result = token1_value < token2_value;
                            break;
                        case TOKEN_LESS_THAN_EQUAL_TO:
                            result = token1_value <= token2_value;
                            break;
                        case TOKEN_GREATER_THAN:
                            result = token1_value > token2_value;
                            break;
                        case TOKEN_GREATER_THAN_EQUAL_TO:
                            result = token1_value >= token2_value;
                            break;
                        default:
                            result = false;
                            break;
                    }
                    return result ? "1" : "0";
                }
                default:
                    printf("Invalid token type for operators\n");
                    return NULL;
            }
            break;
        case LITERAL:
            return node->token->value;
        case VARIABLE:
            char *val = var_table[hash(node->token->value)];
            if (val == NULL) {
                printf("Variable value not set\n");
            }
            return val;
        case ASSIGNMENT:
            if (strcmp(node->token->value, "set") == 0) {
                char *buffer = malloc(sizeof(char) * 11);
                sprintf(buffer, "0");
                var_table[hash(node->children[0]->token->value)] = buffer;
                return buffer;
            }
            if (strcmp(node->token->value, "=") == 0) {
                token_t *token1 = node->children[0]->token;
                if (token1->token_type != TOKEN_VARIABLE) {
                    printf(
                        "Attempting to assign to something that's not a "
                        "variable\n");
                    return NULL;
                }
                int value2 = 0;
                if (node->children[1]->kind == OPERATOR) {
                    char *result =
                        execute_recursive(node->children[1], var_table);
                    value2 = atoi(result);
                } else {
                    token_t *token2 = node->children[1]->token;
                    if (token2->token_type != TOKEN_VARIABLE &&
                        token2->token_type != TOKEN_INT) {
                        printf(
                            "Attempting to assign something that's not a "
                            "variable "
                            "or a literal\n");
                        return NULL;
                    }
                    if (token2->token_type == TOKEN_VARIABLE) {
                        char *val = var_table[hash(token1->value)];
                        if (val == NULL) {
                            printf("Variable not set\n");
                            return NULL;
                        }
                        value2 = atoi(val);
                    } else {
                        value2 = atoi(token2->value);
                    }
                }
                char *buffer = malloc(sizeof(char) * 11);
                sprintf(buffer, "%d", value2);
                var_table[hash(token1->value)] = buffer;
                return buffer;
            }
        case BRANCH:
            break;
        case LOOP: {
            ast_node_t *expr = node->children[0];
            int result = atoi(execute_recursive(expr, var_table));
            while (result == 1) {
                for (int i = 1; i < node->child_count; i++) {
                    ast_node_t *child = node->children[i];
                    execute_recursive(child, var_table);
                }
                result = atoi(execute_recursive(expr, var_table));
            }
            return NULL;
        }
        case PRINT: {
            size_t current_size = sizeof(char) * node->child_count;
            char *result = malloc(current_size);
            result[0] = '\0';
            for (int i = 0; i < node->child_count; i++) {
                char *tmp = execute_recursive(node->children[i], var_table);
                if (tmp == NULL) {
                    continue;
                }
                if (strlen(result) + strlen(tmp) >= current_size) {
                    current_size = (strlen(result) + strlen(tmp)) * 2;
                    char *tmp_realloc = realloc(result, current_size);
                    if (tmp_realloc == NULL) {
                        printf("Failed to realloc print result\n");
                        return NULL;
                    }
                    result = tmp_realloc;
                    strcat(result, tmp);
                }
            }
            printf("STRL-LANG: %s\n", result);
            return NULL;
        }
        default:
            return NULL;
    }
    return NULL;
}

int execute(ast_t *ast) {
    char *var_table[VAR_TABLE_SIZE] = {0};
    for (int i = 0; i < ast->head->child_count; i++) {
        char *val = execute_recursive(ast->head->children[i], var_table);
    }
    for (int i = 0; i < VAR_TABLE_SIZE; i++) {
        free(var_table[i]);
    }
    return 0;
}

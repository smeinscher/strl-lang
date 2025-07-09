#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

static int add_child(ast_node_t *parent, ast_node_t *child) {
    parent->child_count++;
    ast_node_t **tmp =
        realloc(parent->children, sizeof(ast_node_t *) * parent->child_count);
    if (tmp == NULL) {
        printf("Failed to realloc children\n");
        return 2;
    }
    parent->children = tmp;
    parent->children[parent->child_count - 1] = child;
    if (child->parent != NULL) {
        for (int i = 0; i < child->parent->child_count; i++) {
            if (child->parent->children[i] == child) {
                child->parent->children[i] = parent;
                break;
            }
        }
        parent->parent = child->parent;
    }
    child->parent = parent;
    return 0;
}

static int add_to_ast_recursive(ast_node_t *parent, ast_node_t *node) {
    if (parent->child_count == 0) {
        return add_child(parent, node);
    }
    ast_node_t *child = parent->children[parent->child_count - 1];
    if (child->line_number == node->line_number) {
        switch (child->kind) {
            case LITERAL:
            case VARIABLE:
                if (node->kind == LITERAL || node->kind == VARIABLE) {
                    return add_child(parent, node);
                }
                return add_child(node, child);
            case OPERATOR:
                return add_child(child, node);
            case LOOP:
            case BRANCH:
            case ASSIGNMENT:
                return add_to_ast_recursive(child, node);
            case PRINT:
                return add_child(child, node);
            case ROOT:
                printf("Error: Should not reach this\n");
                return 2;
        }
    } else if ((child->kind == LOOP || child->kind == BRANCH) &&
               (node->kind != LOOP && node->kind != BRANCH) &&
               child->scope == node->scope) {
        return add_to_ast_recursive(child, node);
    } else if ((child->kind == LOOP || child->kind == BRANCH) &&
               child->scope != node->scope) {
        if (parent->scope == node->scope) {
            return add_child(parent, node);
        }
        return add_to_ast_recursive(child, node);
    } else {
        return add_child(parent, node);
    }
    printf("Error: Could not add\n");
    return -1;
}

int add_to_ast(ast_t *ast, token_t *token, int line, int scope,
               int parent_scope) {
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->token = token;
    node->parent = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->line_number = line;
    node->scope = scope;
    node->parent_scope = parent_scope;
    switch (token->token_type) {
        case TOKEN_INT:
        case TOKEN_STRING:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
            node->kind = LITERAL;
            break;
        case TOKEN_VARIABLE:
            node->kind = VARIABLE;
            break;
        case TOKEN_SET:
        case TOKEN_SET_EQUAL:
            node->kind = ASSIGNMENT;
            break;
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE:
        case TOKEN_EQUAL:
        case TOKEN_NOT_EQUAL:
        case TOKEN_LESS_THAN:
        case TOKEN_LESS_THAN_EQUAL_TO:
        case TOKEN_GREATER_THAN:
        case TOKEN_GREATER_THAN_EQUAL_TO:
            node->kind = OPERATOR;
            break;
        case TOKEN_WHILE:
        case TOKEN_ENDWHILE:
            node->kind = LOOP;
            break;
        case TOKEN_IF:
        case TOKEN_ENDIF:
            node->kind = BRANCH;
            break;
        case TOKEN_PRINT:
            node->kind = PRINT;
            break;
        default:
            printf("Error: Unknown token type\n");
            return 2;
    }
    if (ast->head == NULL) {
        ast->head = malloc(sizeof(ast_node_t));
        ast->head->token = NULL;
        ast->head->children = malloc(sizeof(ast_node_t *));
        ast->head->child_count = 1;
        ast->head->children[0] = node;
        node->parent = ast->head;
        ast->head->scope = 0;
        ast->head->kind = ROOT;
        ast->head->line_number = 0;
        return 0;
    }
    ast_node_t *current = ast->head;
    return add_to_ast_recursive(current, node);
}

static void free_tree_nodes_recursive(ast_node_t *node) {
    // printf("Free node on line %d\n", node->line_number);
    for (int i = 0; i < node->child_count; i++) {
        free_tree_nodes_recursive(node->children[i]);
    }
    free_token(node->token);
    free(node);
}

void free_tree_nodes(ast_t *ast) {
    for (int i = 0; i < ast->head->child_count; i++) {
        free_tree_nodes_recursive(ast->head->children[i]);
    }
    free(ast->head);
}

static void print_tree_recursive(ast_node_t *node, int level) {
    char *buffer = malloc(sizeof(char) * (level * 2 + 1));
    for (int i = 0; i < level * 2; i++) {
        buffer[i] = '-';
    }
    buffer[level * 2] = '\0';
    printf("%sToken Value: %s; Line Number: %d; Scope: %d\n", buffer,
           node->token->value, node->line_number, node->scope);
    for (int i = 0; i < node->child_count; i++) {
        print_tree_recursive(node->children[i], level + 1);
    }
}

void print_tree(ast_t *ast) {
    for (int i = 0; i < ast->head->child_count; i++) {
        print_tree_recursive(ast->head->children[i], 0);
    }
}

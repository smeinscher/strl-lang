#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    ROOT,
    OPERATOR,
    LITERAL,
    VARIABLE,
    ASSIGNMENT,
    BRANCH,
    LOOP,
    PRINT
} ast_node_kind_t;

typedef struct ast_node {
    token_t *token;
    struct ast_node *parent;
    struct ast_node **children;
    int child_count;
    int line_number;
    int scope;
    int parent_scope;
    ast_node_kind_t kind;
} ast_node_t;

typedef struct ast {
    ast_node_t *head;
} ast_t;

int add_to_ast(ast_t *ast, token_t *token, int line, int scope,
               int parent_scope);

void free_tree_nodes(ast_t *ast);

void print_tree(ast_t *ast);

#endif  // PARSER_H

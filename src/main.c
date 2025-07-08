#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "emitter.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: strl-lang <strl file> [--verbose]");
        return 1;
    }
    bool verbose = argc == 3;
    printf("Starting strl-lang...\n");
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Error opening file: %s", argv[1]);
        return 2;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = malloc(sizeof(char) * (file_size + 1));
    if (source == NULL) {
        printf("Error allocating memory for source buffer\n");
        fclose(file);
        return 3;
    }
    fread(source, sizeof(char), file_size, file);
    fclose(file);
    source[file_size] = '\0';

    ast_t ast;
    ast.head = NULL;
    int current_line = 1;
    int current_scope = 0;
    int current_text_size = 0;
    bool start_line = true;
    token_t *tokens_head = NULL;
    token_t *tokens_current = NULL;
    for (int i = 0; i < file_size; i++) {
        if (source[i] == '\n') {
            current_text_size = 0;
            start_line = true;
            current_line++;
            continue;
        }
        while (start_line && (source[i] == ' ' || source[i] == '\t')) {
            i++;
        }
        start_line = false;
        current_text_size++;
        // Comment line; skip processing
        if (source[i] == '#') {
            while (i < file_size && source[i] != '\n') {
                i++;
            }
            current_text_size = 0;
            current_line++;
            continue;
        }
        char *text = NULL;
        // String; add everything inside of it to same token
        if (source[i] == '"') {
            int length = 0;
            do {
                length++;
            } while (source[i + length] != '"');
            length++;
            text = malloc(sizeof(char) * length + 1);
            if (text == NULL) {
                printf("Failed to malloc text\n");
                return 4;
            }
            int end = i + length;
            int index = 0;
            while (i < end) {
                text[index] = source[i];
                i++;
                index++;
            }
            text[length] = '\0';
            current_text_size = 0;
        }
        if (text != NULL) {
            token_t *token = text_to_token(text);
            if (tokens_head == NULL) {
                tokens_head = token;
            } else {
                tokens_current->next_token = token;
            }
            tokens_current = token;
            current_text_size = 0;
            text = NULL;
        }
        if (current_text_size != 0 && (source[i] == ' ' || source[i] == ';')) {
            if (i == file_size - 1) {
                continue;
            }
            text = malloc(sizeof(char) * current_text_size + 1);
            if (text == NULL) {
                printf("Failed to malloc text\n");
                return 4;
            }
            for (int j = i - current_text_size + 1, k = 0; j < i; j++, k++) {
                text[k] = source[j];
            }
            text[current_text_size - 1] = '\0';
        }
        if (text != NULL) {
            token_t *token = text_to_token(text);
            if (tokens_head == NULL) {
                tokens_head = token;
            } else {
                tokens_current->next_token = token;
            }
            tokens_current = token;
            current_text_size = 0;
            text = NULL;
        }
        if (source[i] == ';') {
            token_t *current = tokens_head;
            while (current != NULL) {
                if (verbose) {
                    printf("Token Type: %d, Value: (%s), Length: %d\n",
                           current->token_type, current->value,
                           current->length);
                }
                if (current->token_type == TOKEN_WHILE ||
                    current->token_type == TOKEN_IF) {
                    current_scope++;
                } else if (current->token_type == TOKEN_ENDWHILE ||
                           current->token_type == TOKEN_ENDIF) {
                    current_scope--;
                    current = current->next_token;
                    continue;
                }
                int result =
                    add_to_ast(&ast, current, current_line, current_scope);
                if (result != 0) {
                    printf("Error: Invalid syntax at line %d\n", current_line);
                    return 5;
                }
                current = current->next_token;
            }
            tokens_head = NULL;
            tokens_current = NULL;
            start_line = true;
            if (verbose) {
                printf("--- Statement End ---\n");
            }
        }
    }
    execute(&ast);
    if (verbose) {
        print_tree(&ast);
    }
    free_tree_nodes(&ast);
    printf("Finished execution.\n");
    return 0;
}

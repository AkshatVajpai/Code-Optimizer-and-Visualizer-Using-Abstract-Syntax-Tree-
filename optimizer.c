#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


ASTNode* deep_copy_ast(ASTNode* node) {
    if (!node) return NULL;

    ASTNode* copy = create_node(node->type, node->value);
    copy->left = deep_copy_ast(node->left);
    copy->right = deep_copy_ast(node->right);
    copy->next = deep_copy_ast(node->next);
    return copy;
}


ASTNode* fold_constants(ASTNode* node) {
    if (!node) return NULL;

    node->left = fold_constants(node->left);
    node->right = fold_constants(node->right);
    node->next = fold_constants(node->next);

    if (node->type == NODE_BINOP && node->left && node->right &&
        node->left->type == NODE_INT && node->right->type == NODE_INT) {

        int left_val = atoi(node->left->value);
        int right_val = atoi(node->right->value);
        int result = 0;
        char op = node->value[0];

        switch (op) {
            case '+': result = left_val + right_val; break;
            case '-': result = left_val - right_val; break;
            case '*': result = left_val * right_val; break;
            case '/': 
                if (right_val != 0)
                    result = left_val / right_val; 
                else 
                    return node; 
                break;
            case '<': result = left_val < right_val; break;
            case '>': result = left_val > right_val; break;
            case 'L': result = left_val <= right_val; break;
            case 'G': result = left_val >= right_val; break;
            case '=': result = left_val == right_val; break;
            case '!': result = left_val != right_val; break;
            case '&': result = left_val && right_val; break;
            case '|': result = left_val || right_val; break;
            default: return node;
        }

        ASTNode* folded = make_int_node(result);
        free_ast(node);
        return folded;
    }

    if (node->type == NODE_UNARY && node->left && node->left->type == NODE_INT) {
        int val = atoi(node->left->value);
        int result = 0;

        if (strcmp(node->value, "!") == 0) {
            result = !val;
            ASTNode* folded = make_int_node(result);
            free_ast(node);
            return folded;
        } else if (strcmp(node->value, "++") == 0 || strcmp(node->value, "--") == 0) {
            // Don't fold increment/decrement operators as they have side effects
            return node;
        }
    }

    return node;
}


ASTNode* eliminate_dead_code(ASTNode* node) {
    if (!node) return NULL;

    // Eliminate if(0) statements
    if (node->type == NODE_IF &&
        node->left && node->left->type == NODE_INT &&
        strcmp(node->left->value, "0") == 0) {
        free_ast(node);
        return NULL;
    }

    // Eliminate if(1) statements
    if (node->type == NODE_IF &&
        node->left && node->left->type == NODE_INT &&
        strcmp(node->left->value, "1") == 0) {
        ASTNode* result = node->right;
        node->right = NULL;
        free_ast(node);
        return result;
    }

    // Handle sequence nodes
    if (node->type == NODE_SEQ) {
        node->left = eliminate_dead_code(node->left);

        // If left is a return statement, eliminate right
        if (node->left && node->left->type == NODE_RETURN) {
            free_ast(node->right);
            node->right = NULL;
        } else {
            node->right = eliminate_dead_code(node->right);
        }
    } else {
        node->left = eliminate_dead_code(node->left);
        node->right = eliminate_dead_code(node->right);
        node->next = eliminate_dead_code(node->next);
    }

    return node;
}


ASTNode* unroll_loops(ASTNode* node) {
    if (!node) return NULL;

    node->left = unroll_loops(node->left);
    node->right = unroll_loops(node->right);
    node->next = unroll_loops(node->next);

    if (node->type == NODE_FOR && node->left && node->right) {
        ASTNode* init = node->left;
        ASTNode* cond = node->right;
        ASTNode* update = cond->next;
        ASTNode* body = update ? update->next : NULL;

        if (!init || !cond || !update || !body) return node;

        // Check if it's a simple counting loop
        if (init->type == NODE_DECL && init->left && init->left->type == NODE_INT &&
            cond->type == NODE_BINOP && cond->right && cond->right->type == NODE_INT &&
            strcmp(update->value, "++") == 0) {

            int start = atoi(init->left->value);
            int end = atoi(cond->right->value);

            // Only unroll if the number of iterations is small
            if (end - start <= 10) {
                ASTNode* unrolled = NULL;
                for (int i = start; i < end; i++) {
                    ASTNode* cloned = deep_copy_ast(body);
                    if (unrolled)
                        unrolled = make_seq_node(unrolled, cloned);
                    else
                        unrolled = cloned;
                }

                free_ast(node);
                return unrolled;
            }
        }
    }

    return node;
}

ASTNode* optimize_ast(ASTNode* root) {
    if (!root) return NULL;

    // Apply optimizations in sequence
    root = fold_constants(root);
    root = eliminate_dead_code(root);
    root = unroll_loops(root);

    return root;
}


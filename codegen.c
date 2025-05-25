#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_code(ASTNode* node, FILE* output, int indent) {
    if (!node) return;

    // Print indentation
    for (int i = 0; i < indent; i++) {
        fprintf(output, "    ");
    }

    switch (node->type) {
        case NODE_FUNC_DEF:
            fprintf(output, "int %s() {\n", node->value);
            generate_code(node->left, output, indent + 1);
            fprintf(output, "}\n");
            break;

        case NODE_SEQ:
            generate_code(node->left, output, indent);
            generate_code(node->right, output, indent);
            break;

        case NODE_DECL:
            fprintf(output, "int %s", node->value);
            if (node->left) {
                fprintf(output, " = ");
                generate_code(node->left, output, 0);
            }
            fprintf(output, ";\n");
            break;

        case NODE_BINOP:
            fprintf(output, "(");
            generate_code(node->left, output, 0);
            fprintf(output, " %s ", node->value);
            generate_code(node->right, output, 0);
            fprintf(output, ")");
            break;

        case NODE_UNARY:
            if (strcmp(node->value, "++") == 0) {
                generate_code(node->left, output, 0);
                fprintf(output, "++");
            } else if (strcmp(node->value, "--") == 0) {
                generate_code(node->left, output, 0);
                fprintf(output, "--");
            }
            break;

        case NODE_IF:
            fprintf(output, "if (");
            generate_code(node->left, output, 0);
            fprintf(output, ") {\n");
            generate_code(node->right, output, indent + 1);
            fprintf(output, "}");
            if (node->next) {
                fprintf(output, " else {\n");
                generate_code(node->next, output, indent + 1);
                fprintf(output, "}");
            }
            fprintf(output, "\n");
            break;

        case NODE_FOR:
            fprintf(output, "for (");
            generate_code(node->left, output, 0);
            fprintf(output, "; ");
            generate_code(node->right, output, 0);
            fprintf(output, "; ");
            if (node->right && node->right->next) {
                generate_code(node->right->next, output, 0);
            }
            fprintf(output, ") {\n");
            if (node->right && node->right->next && node->right->next->next) {
                generate_code(node->right->next->next, output, indent + 1);
            }
            fprintf(output, "}\n");
            break;

        case NODE_FUNC_CALL:
            fprintf(output, "%s(", node->value);
            if (node->left) {
                generate_code(node->left, output, 0);
            }
            fprintf(output, ")");
            break;

        case NODE_EXPR_LIST:
            generate_code(node->left, output, 0);
            if (node->next) {
                fprintf(output, ", ");
                generate_code(node->next, output, 0);
            }
            break;

        case NODE_RETURN:
            fprintf(output, "return ");
            generate_code(node->left, output, 0);
            fprintf(output, ";\n");
            break;

        case NODE_INT:
            fprintf(output, "%s", node->value);
            break;

        case NODE_STRING:
            fprintf(output, "\"%s\"", node->value);
            break;

        case NODE_VAR:
            fprintf(output, "%s", node->value);
            break;

        default:
            break;
    }
}

void generate_c_code(ASTNode* root, const char* output_file) {
    FILE* output = fopen(output_file, "w");
    if (!output) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    // Add necessary includes
    fprintf(output, "#include <stdio.h>\n\n");
    
    // Generate the code
    generate_code(root, output, 0);
    
    fclose(output);
} 
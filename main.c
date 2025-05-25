#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern FILE* yyin;              
extern ASTNode* ast_root;

int main(int argc, char *argv[]) {
    printf("[DEBUG] Program started.\n");
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Open input file
    yyin = fopen(argv[1], "r");   
    if (!yyin) {
        perror(argv[1]);
        return 1;
    }
    printf("[DEBUG] Opened input file: %s\n", argv[1]);

    // Open output file for AST
    FILE* out = fopen("output.txt", "w");
    if (!out) {
        perror("output.txt");
        return 1;
    }

    // Parse the input file
    yyparse();

    // Print original AST
    fprintf(out, "Original AST:\n");
    print_ast(ast_root, out, 0);

    // Optimize the AST
    ast_root = optimize_ast(ast_root);
    
    // Print optimized AST
    fprintf(out, "\nOptimized AST:\n");
    print_ast(ast_root, out, 0);

    // Generate C code from the optimized AST
    generate_c_code(ast_root, "regenerated.c");

    // Cleanup
    fclose(yyin);
    fclose(out);

    printf("AST saved to output.txt\n");
    printf("Regenerated C code saved to regenerated.c\n");
    return 0;
}
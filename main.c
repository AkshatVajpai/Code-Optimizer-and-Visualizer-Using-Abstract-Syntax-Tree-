#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern FILE* yyin;              
extern ASTNode* ast_root;

int main() {
    // Open input file
    yyin = fopen("input.c", "r");   
    if (!yyin) {
        perror("input.c");
        return 1;
    }

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
# Code Optimizer and Visualizer Using Abstract Syntax Tree (AST)

A C-based project that implements an Abstract Syntax Tree (AST) parser, optimizer, and code generator. This tool can parse C code, generate an AST representation, optimize the code structure, and regenerate C code from the optimized AST.

## Features

- **AST Generation**: Parses C code and creates an Abstract Syntax Tree representation
- **Code Optimization**: Optimizes the AST structure for better code efficiency
- **Code Generation**: Regenerates C code from the optimized AST
- **Support for Common C Constructs**:
  - Function definitions and calls
  - Variable declarations and assignments
  - Binary operations
  - Unary operations (++, --)
  - Control structures (if-else, for loops)
  - Return statements
  - Expression lists
  - String and integer literals

## Project Structure

```
.
├── ast.h          # AST node definitions and function declarations
├── ast.c          # AST implementation and core functionality
├── codegen.c      # Code generation from AST
└── README.md      # This file
```

## Implementation Details

### AST Node Types

The project supports various node types including:
- Function definitions and calls
- Variable declarations
- Binary operations
- Unary operations
- Control structures
- Return statements
- Expression lists
- Literals (integers, strings)

### Code Generation

The code generator (`codegen.c`) handles:
- Proper indentation and formatting
- C code structure preservation
- Expression evaluation
- Control flow statements
- Function definitions and calls

## Usage

1. Include the necessary headers:
```c
#include "ast.h"
```

2. Create AST nodes using the provided constructors:
```c
ASTNode* make_int_node(int value);
ASTNode* make_string_node(char* value);
ASTNode* make_var_node(char* name);
// ... and more
```

3. Build your AST structure using the node constructors and linking functions:
```c
void add_child(ASTNode* parent, ASTNode* child);
void add_sibling(ASTNode* node, ASTNode* sibling);
```

4. Optimize the AST:
```c
ASTNode* optimize_ast(ASTNode* root);
```

5. Generate C code from the AST:
```c
void generate_c_code(ASTNode* root, const char* output_file);
```

## Example

```c
// Create a simple function definition
ASTNode* var = make_var_node("x");
ASTNode* init = make_int_node(5);
ASTNode* decl = make_decl_node("x", init);
ASTNode* ret = make_return_node(var);
ASTNode* body = make_seq_node(decl, ret);
ASTNode* func = make_function_node("main", body);

// Generate C code
generate_c_code(func, "output.c");
```

## Contributing

Feel free to submit issues and enhancement requests!

## License

<<<<<<< HEAD
This project is open source and available under the MIT License.
=======
This project is open source .
>>>>>>> 2a79376828a106b071132788680936e5b96ffbe3

## Author

Akshat Vajpai
<<<<<<< HEAD
=======

>>>>>>> 2a79376828a106b071132788680936e5b96ffbe3


# TODO

- Finish SyntaxTree.dump()
- Add typechecking
- Add const to everything
- Add failure tests

# Plan
- Finish parser
    - Expressions
        - Char const (ast)
        - Str const
        - Array index
        - Binary operators (ast)
        - Unary operators (ast)
        - Function call (ast)
        - Assignment (ast)
        - Floats
        - Increment operations
    - Statements
        - Array declaration
        - Global variables
        - Function prototypes
        - While loops
        - For loops
    - Misc
        - Type conversion nodes

- Convert the AST into a three address code IR
- Construct a control flow graph using the IR
- Generate LLVM using control flow graph

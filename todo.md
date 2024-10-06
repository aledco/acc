# TODO

- Add const to everything
- Add failure tests
- Free temps in ir codegen
- Need to add spans to AST nodes to show error messages better

# Plan
- Finish parser
    - Expressions
        - Char const
        - Str const
        - Array index
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

- Construct a control flow graph using the IR
- Generate LLVM using control flow graph

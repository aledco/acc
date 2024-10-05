# TODO

- Finish SyntaxTree.dump()
- Add a stack for symbol tables in ParserContext
- Add typechecking
- Add const to everything
- Add failure tests
- Add a return statement to the end of a function if there is none
- Free temps in ir codegen
- Fix segfault for IR dump test11.c

# Plan
- Finish parser
    - Expressions
        - Char const (ast)
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

- Convert the AST into a three address code IR
- Construct a control flow graph using the IR
- Generate LLVM using control flow graph

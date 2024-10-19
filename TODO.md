# TODO

## Parser
- Need to add spans to AST nodes to show error messages better
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

## IR
- Free temps in ir codegen
- Add optimization

## Code Maintenance
- Add const to everything possible
- Comment functions

## Tests
- Add failure tests
- Add more tests
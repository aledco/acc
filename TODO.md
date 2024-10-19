# TODO

## Lexer
- Normalize input before lexing
- Lex comments

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
- Add type casting

## IR
- Add optimization

## Code Maintenance
- Add const to everything possible

## Tests
- Add failure tests
- Add more tests

## Misc
- Clean up TODOs
- Use specific exceptions for each type of error, have a try catch in main to print errors
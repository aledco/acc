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
        - If statements
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
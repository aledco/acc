# TODO

## Lexer
- Normalize input before lexing

## Parser
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
        - While loops
        - For loops
    - Misc
        - Type conversion nodes
- Add type casting

## IR
- Add optimization

## LLVM
- Figure out how to modify parameters
    - Clang allocates space on the stack for each parameter

## Code Maintenance
- Add const to everything possible

## Tests
- Add failure tests
- Add tests from CSC 553 compiler

## Misc
- Clean up TODOs
- Add CI, start using branches
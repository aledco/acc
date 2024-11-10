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
        - Bitwise operations (and the rest of the C operators)
        - Pointers
    - Statements
        - Array declaration
    - Misc
        - Type conversion nodes
- Add type casting

## IR
- Add optimization

## LLVM

## Code Maintenance
- Add const to everything possible

## Tests
- Add failure tests
- Add tests from CSC 553 compiler
    - First will need to add arrays, strings, and global vars
    - Probably need support for multiple global vars in one decl

## Benchmarking
- Add benchmarks and compare speed of generated code between clang and acc

## Misc
- Clean up TODOs
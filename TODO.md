# TODO

## Lexer
- Normalize input before lexing

## Parser
- Finish parser
    - Expressions
        - Char const
        - Str const
        - Floats
        - Bitwise operations (and the rest of the C operators)
        - Pointers
    - Misc
        - Type conversion nodes
        - Structs (maybe)
- Add type casting

## IR
- Use addr and deref instead of index
    - update IR.md
- Add optimization

## LLVM

## Code Maintenance

## Tests
- Add failure tests
- Add tests from CSC 553 compiler
    - First will need to add arrays, strings, and global vars
    - Probably need support for multiple global vars in one decl

## Benchmarking
- Add benchmarks and compare speed of generated code between clang and acc

## Misc
- Clean up TODOs
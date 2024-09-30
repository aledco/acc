# TODO

- Fix SyntaxTree.dump()
- Write typechecking
- Add const to everything
- May need to add an IR between AST and LLVM
- Add errors

# Plan
- Finish parser
- Convert the AST into a three address code IR
- Construct a control flow graph using the IR
- Generate LLVM using control flow graph

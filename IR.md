# Description
The IR used in this compiler is a three-address code IR. Each instruction is a quad consisting of an optional operator, 2 optional arguments, and a result. It is used to create a CFG from the source code before being converted to LLVM IR.

# Instructions
Following is a description of the instructions available in the IR.

## Data Operations
- global x type 
    - Declare a global variable `x` with `type`
- string str
    - Declare a string `str`

## Binary Operations
- x = y op z
    - Perform the operation `op` on `y` and `z` and store the result in `x`
    - `op` can be `+, -, *, /, %`
    - `y` and `z` are constants or variables

## Unary Operations
- x =  op y
    - Perform the operation `op` on `y` and store the result in `x`
    - `op` can be `-, *, &`
    - `y` is a constant or variable
- x = y
    - Copy `y` into `x`
    - `y` is a constant or variable

## Index Operations
- a[i] = x
    - Set the value in array `a` at index `i` to `x`
 - x = a[i]
    - Set `x` to the value in array `a` at index `i`

## Branch Operations
- label x
    - Create a label called `x`
- goto label
    - Jump to `label`
- if (x op y) goto label
    - Perform the operation `op` on `x` and `y`. If the result is true, goto `label`
    - `op` can be `==, !=, <, <=, >, >=`
     - `x` and `y` are constants or variables

## Procedural Operations
- enter f
    - Enter the function `f`
- return x
    - Return `x` from the function
    - `x` is an optional constant or variable
- param x 
    - Create a parameter from `x` before calling a function
    - `x` is a constant or variable
- call f, n
    - Call function `f` with `n` arguments
- retrieve x
    - Store the return value of a function call into `x`

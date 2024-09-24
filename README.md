# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc).

# Dependencies
All instructions are assuming an Ubuntu environment.

## Install LLVM
```
sudo apt install llvm
```

To test that LLVM is installed correctly run
```
make toy
./toy
ready> def foo(a b) a*a + 2*a*b + b*b;
```

# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc).

# Dependencies
All instructions are asuming an Ubuntu environment.

## Install flex and bison
```
sudo apt install flex bison
```

## Install LLVM
```
sudo apt install llvm
```

## Test everything was setup correctly
```
make toy
./toy
ready> def foo(a b) a*a + 2*a*b + b*b;
```


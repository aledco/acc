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

# Testing

## Install Google Test
Follow instructions in [How to set up Google Test](https://stackoverflow.com/questions/13513905/how-to-set-up-googletest-as-a-shared-library-on-linux)

## Build and Run the Tests
```
make test
./test
```



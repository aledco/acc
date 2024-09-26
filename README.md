# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc).

# Dependencies
All instructions are assuming an Ubuntu 22 environment.

## Install LLVM
```
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" -- -n jammy 18
```

## Install clang
```
sudo apt install clang
```

To test that LLVM and clang is installed correctly run
```
make toy
./toy
ready> def foo(a b) a*a + 2*a*b + b*b;
```

You may need to run the following command to use the correct llvm-config
```
sudo ln -s /usr/bin/llvm-config-18 /usr/bin/llvm-config
```

# Testing

## Install Google Test
Follow instructions in [How to set up Google Test](https://stackoverflow.com/questions/13513905/how-to-set-up-googletest-as-a-shared-library-on-linux)

## Build and Run the Tests
```
make test
./test
```



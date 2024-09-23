# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc).

# Dependencies
All instructions are asuming an Ubuntu environment.

## Install flex and bison
```
sudo apt install flex bison
```

## To install LLVM
Instructions are adapted from https://llvm.org/docs/GettingStarted.html.
 
```
git clone --depth 1 https://github.com/llvm/llvm-project.git
cd llvm-project
sudo apt install ninja-build
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja -C build check-llvm
```


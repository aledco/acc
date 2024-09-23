# acc
Another C compiler

## To install LLVM
Instructions adapted from https://llvm.org/docs/GettingStarted.html
 
```
git clone --depth 1 https://github.com/llvm/llvm-project.git
cd llvm-project
# sudo apt install ninja-build
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja -C build check-llvm
```


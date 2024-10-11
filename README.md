# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc). It does not implement all of the C standard, and is not meant for real world use.

# Dependencies
The following commands are assuming an Ubuntu environment with codename jammy.

### Install LLVM
```
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" -- -n jammy 18
```

### Install clang
```
sudo apt install clang
```

### Install TCLAP
```
sudo apt install libtclap-dev
```

## LLVM Documentation
- [LLVM Language Reference Manual](https://llvm.org/docs/LangRef.html)
- [llvm::IRBuilderBase Class Reference](https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html)

# Testing

### Install Google Test
Follow instructions in [How to set up Google Test](https://stackoverflow.com/questions/13513905/how-to-set-up-googletest-as-a-shared-library-on-linux).

### Build and Run the Tests
```
make test
./test
```

# VS Code Settings
Run the command **C/C++: Edit Configurations (UI)** using the Command Palette (`Ctrl+Shift+P`) to open the C++ configuration settings.

### Compiler Path
Set the Compiler path to `/usr/bin/clang++-18`.

### Include Path
In the Include path text box, and add `/usr/lib/llvm-18/include` on a new line.

### C++ Standard
Set the C++ Standard to `C++17`.

# Miscellaneous
- [IR Documentation](./IR.md)
- [TODO](./TODO.md)
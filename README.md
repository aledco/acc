# Description
This is another C compiler (acc), or Alexander Cooper's compiler (acc).

# Dependencies
All instructions are assuming an Ubuntu environment with codename jammy.

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

## Install TCLAP
```
sudo apt-get install libtclap-dev
```

# Testing

## Install Google Test
Follow instructions in [How to set up Google Test](https://stackoverflow.com/questions/13513905/how-to-set-up-googletest-as-a-shared-library-on-linux)

## Build and Run the Tests
```
make test
./test
```

# VS Code Settings
Run the command **C/C++: Edit Configurations (UI)** using the Command Palette (`Ctrl+Shift+P`) to open the C++ configuration settings.

## Compiler Path
Set the Compiler path to `/usr/bin/clang++-18`.

## Include Path
In the Include path text box, and add `/usr/lib/llvm-18/include` on a new line.

## C++ Standard
Set the C++ Standard to `C++17`.

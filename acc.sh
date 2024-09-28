#!/bin/bash

# for now, this bash script is used to compile a c file to an object file / executable

rm -rf acc-out
mkdir acc-out

./acc $1 2> acc-out/a.ll
llc-18 acc-out/a.ll -o acc-out/a.s
clang++-18 acc-out/a.s -o acc-out/a.out


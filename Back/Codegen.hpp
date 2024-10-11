#pragma once

#include <iostream>
#include "SyntaxTree.hpp"
#include "Quad.hpp"
#include "CFG.hpp"

void codegen(std::shared_ptr<Program> program, std::ostream *file = nullptr);

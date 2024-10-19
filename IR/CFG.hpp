#pragma once

#include <memory>
#include "Quad.hpp"

struct BBList;

/**
 * Represents a basic block.
 */
struct BasicBlock
{
    QuadList qlist;
    std::vector<BasicBlock *> in;
    std::vector<BasicBlock *> out;
    BasicBlock(std::shared_ptr<Quad> first, std::shared_ptr<Quad> last) : qlist(QuadList(first, last)) {}

    inline void link(std::shared_ptr<BasicBlock> to)
    {
        out.push_back(to.get());
        to->in.push_back(this); 
    }
};

std::vector<std::shared_ptr<BasicBlock>> ConstructCFG(QuadList& qlist);
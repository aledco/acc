#include <cassert>
#include <string>
#include "CFG.hpp"

static std::shared_ptr<BasicBlock> find_next_block(std::shared_ptr<Quad> instructions);
static std::vector<std::shared_ptr<BasicBlock>> find_all_blocks(std::shared_ptr<Quad> instructions);
static std::shared_ptr<BasicBlock> find_block_for_label(std::vector<std::shared_ptr<BasicBlock>>& blocks, std::string label);
static void link_blocks(std::vector<std::shared_ptr<BasicBlock>>& blocks);

std::vector<std::shared_ptr<BasicBlock>> ConstructCFG(QuadList& qlist)
{
    auto blocks = find_all_blocks(qlist.get_head());
    link_blocks(blocks);
    return blocks;
}

static std::shared_ptr<BasicBlock> find_next_block(std::shared_ptr<Quad> instructions)
{
    auto first = instructions;
    std::shared_ptr<Quad> last;
    for (auto curr = instructions; curr != nullptr; curr = curr->next)
    {
        switch (curr->op)
        {
            case QuadOp::Goto:
            case QuadOp::IfEq:
            case QuadOp::IfNeq:
            case QuadOp::IfLt:
            case QuadOp::IfLeq:
            case QuadOp::IfGt:
            case QuadOp::IfGeq:
                return std::make_shared<BasicBlock>(first, curr);
            default:
                break;
        }

        if (curr->next == nullptr || curr->next->op == QuadOp::Label)
        {
            return std::make_shared<BasicBlock>(first, curr);
        }
    }

    assert(false);
    return nullptr;
}

static std::vector<std::shared_ptr<BasicBlock>> find_all_blocks(std::shared_ptr<Quad> instructions)
{
    std::vector<std::shared_ptr<BasicBlock>> blocks;
    while (instructions != nullptr)
    {
        auto block = find_next_block(instructions);
        blocks.push_back(block);
        instructions = block->qlist.get_tail()->next;
    }

    return blocks;
}

static std::shared_ptr<BasicBlock> find_block_for_label(std::vector<std::shared_ptr<BasicBlock>>& blocks, std::string label)
{
    for (auto& block : blocks)
    {
        auto first = block->qlist.get_head();
        if (first->op == QuadOp::Label && first->arg1->strconst == label)
        {
            return block;
        }
    }

    assert(false);
    return nullptr;
}

static void link_blocks(std::vector<std::shared_ptr<BasicBlock>>& blocks)
{
    for (auto i = 0; i < blocks.size(); i++)
    {
        auto block = blocks[i];
        auto last = block->qlist.get_tail();
        std::shared_ptr<BasicBlock> block_for_label;
        switch (last->op)
        {
            case QuadOp::Goto:
                block_for_label = find_block_for_label(blocks, last->arg1->strconst);
                block->link(block_for_label);
                continue;
            case QuadOp::IfEq:
            case QuadOp::IfNeq:
            case QuadOp::IfLt:
            case QuadOp::IfLeq:
            case QuadOp::IfGt:
            case QuadOp::IfGeq:
                block_for_label = find_block_for_label(blocks, last->res->strconst);
                block->link(block_for_label);
                break;
            default:
                break;
        }

        if (i < blocks.size() - 1)
        {
            block->link(blocks[i+1]);
        }
    }
}

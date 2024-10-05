#include <memory>
#include "SymbolTable.hpp"

struct IRContext // TOOD add this to IRCodegen
{
    std::shared_ptr<SymbolTable> symbol_table;
};

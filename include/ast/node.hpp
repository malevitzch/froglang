#include <llvm/IR/Value.h>
class Node {
    virtual llvm::Value* codegen() = 0;
};

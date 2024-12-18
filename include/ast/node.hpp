#pragma once
#include <llvm/IR/Value.h>

// Abstract class
class Node {
private:
    std::string name = "Uninitialized Name";
protected:
    Node() = default;
    Node(std::string name);
public:
    virtual llvm::Value* codegen() = 0;
    std::string get_name();
};

class GlobjectNode : public Node {
private:
public:
    llvm::Value* codegen() override;
    GlobjectNode();
};

class ProgramNode : public Node {
private:
    std::vector<Node*> globjects;
public:
    llvm::Value* codegen() override;
    void add_obj(GlobjectNode* globject);
    ProgramNode();
};

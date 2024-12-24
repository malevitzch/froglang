#include "ast/node.hpp"

namespace ast {
  
  class ExprNode : public Node {
  private:
    // TODO: type might eventually become something more complicated than a string
    std::string type;
  public:
    ExprNode(std::string type);
  };

  class BinaryOperator : public ExprNode {

  };

  class IntegerConstant : public ExprNode {
  private:
    long long value;
    // TODO: this might be replaced by an arbitrarily-large bit array
    // so that we can have arbitrary size integers later
  public:
    IntegerConstant(std::string data, std::string type);
    llvm::Value* codegen() override;
  };

}

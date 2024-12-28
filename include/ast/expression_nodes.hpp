#include "ast/node.hpp"

namespace ast {
  
  class ExprNode : public Node {
  private:
  protected:
    // TODO: Type might eventually become something more complicated than a string
    std::string type;
  public:
    virtual std::string get_name() override;
    virtual std::string get_type();
    ExprNode(std::string type);

    //TODO: Decide whether or not this should stay. It's very practical but not very pure oop-wise
    ExprNode() = default;
  };

  class BinaryOperator : public ExprNode {
  private:
    std::string operator_type;
  public: 
    BinaryOperator(std::string operator_type);
    virtual llvm::Value* codegen() override;
    virtual std::string get_type() override;
  };

  class IntegerConstant : public ExprNode {
  private:
    long long value;
    // TODO: This might be replaced by an arbitrarily-large bit array
    // so that we can have arbitrary size integers later
  public:
    IntegerConstant(std::string data, std::string type);
    llvm::Value* codegen() override;
  };

}

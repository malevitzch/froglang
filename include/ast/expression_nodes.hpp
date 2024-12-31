#include "ast/node.hpp"

namespace ast {
  
  class ExprNode : public StatementNode {
  private:
  protected:
    ExprNode() = default;
    ~ExprNode() = default;
    // TODO: Type might eventually become something more complicated than a string
    std::string type;
  public:
    virtual std::string get_name() override;
    virtual std::string get_type();
    ExprNode(std::string type);

    //TODO: Decide whether or not this should stay. It's very practical but not very pure oop-wise
  };

  class BinaryOperator : public ExprNode {
  private:
    std::string operator_type;
    std::shared_ptr<ExprNode> left, right;
  public: 
    BinaryOperator(std::string operator_type, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right);
    virtual llvm::Value* codegen() override;
    virtual std::string get_type() override;
    virtual std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class IntegerConstant : public ExprNode {
  private:
    long long value;
    // TODO: This might be replaced by an arbitrarily-large bit array
    // so that we can have arbitrary size integers later
  public:
    IntegerConstant(std::string data, std::string type);
    llvm::Value* codegen() override;
    IntegerConstant() = default;
  };

}

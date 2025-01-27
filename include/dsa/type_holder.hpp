#include <map>
#include <llvm/IR/Type.h>

namespace DSA {
  class TypeHolder {
  private:
    std::map<std::string, llvm::Type*> types;
  public:
    bool contains(std::string type_name);
    void add_type(std::string type_name, llvm::Type* type);
    llvm::Type* get_type(std::string type_name);
  };
}

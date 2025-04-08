#include <string>
#include <functional>
#include <map>

#include "llvm/IR/Value.h"

namespace frtype {

  class VariableType {
  private:
    const std::string name;
    const bool mutable_type = false;
    const bool autocast_type = false;
    std::map<
      const std::shared_ptr<VariableType>,
      const std::function<llvm::Value*(llvm::Value*)>> casts;
  public:
    VariableType(std::string name, bool mutable_type, bool autocast_type);
    virtual std::string get_name() const;
    virtual bool is_automatically_castable() const;
    virtual bool is_mutable() const;
  }; 
}

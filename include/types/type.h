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
    std::string get_name() const;
    bool is_automatically_castable() const;
    bool is_mutable() const;
  };

}

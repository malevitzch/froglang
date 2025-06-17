#ifndef TYPES_TYPE_HPP
#define TYPES_TYPE_HPP

#include <string>
#include <functional>
#include <map>

#include "llvm/IR/Value.h"

namespace frtype {

  class Type {
  private:
    const std::string name;
    llvm::Type* underlying_type;
    std::map<
      const std::shared_ptr<Type>,
      const std::function<llvm::Value*(llvm::Value*)>> casts;
  public:
    Type(std::string name, llvm::Type* underlying_type);
    virtual std::string get_name() const;
    llvm::Type* get_underlying_type() const;
  }; 
}

#endif

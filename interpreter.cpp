#include <interpreter.h>

std::variant<std::monostate, double, bool, std::string> Interpreter::evaluate(const std::shared_ptr<Expr> &expr){
    return expr->accept(this);
}



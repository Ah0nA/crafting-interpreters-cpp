#include <interpreter.h>
#include <stdexcept>
#include <iostream>

std::variant<std::monostate, double, bool, std::string> Interpreter::evaluate(const std::shared_ptr<Expr> &expr)
{
    return expr->accept(this);
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Expr>> &expressions)
{
    try
    {
        for (const auto &expr : expressions)
        {
            evaluate(expr);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Interpreter Error: " << e.what() << std::endl;
    }
}

std::variant<std::monostate, double, bool, std::string> evaluateUnaryExpr(const Unary *expr){
    auto right = Interpreter::evaluate(expr.right);

}

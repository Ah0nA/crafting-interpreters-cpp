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
            auto value = evaluate(expr);
            if (std::holds_alternative<std::string>(value))
                std::cout << std::get<std::string>(value) << std::endl;
            else if (std::holds_alternative<double>(value))
                std::cout << std::get<double>(value);
            else if (std::holds_alternative<bool>(value))
                std::cout << (std::get<bool>(value) ? "true" : "false") << std::endl;
            else
                std::cout << "nil" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Interpreter Error: " << e.what() << std::endl;
    }
}

std::variant<std::monostate, double, bool, std::string> Interpreter::visitUnaryExpr(Unary *expr)
{
    auto right = evaluate(expr->right);

    switch (expr->op.type)
    {
    case TokenType::MINUS:
        checkNumberOperand(expr->op, right);
        return -std::get<double>(right);
    case TokenType::BANG:
        return !isTruthy(right);
    default:
        return {};
    }
}

std::variant<std::monostate, double, bool, std::string> Interpreter::visitBinaryExpr(Binary *expr)
{
    auto left = evaluate(expr->left);
    auto right = evaluate(expr->right);

    switch (expr->op.type)
    {
    case TokenType::PLUS:
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
            return std::get<double>(left) + std::get<double>(right);
        if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
            return std::get<std::string>(left) + std::get<std::string>(right);
        break;
    case TokenType::MINUS:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) - std::get<double>(right);
    case TokenType::STAR:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) * std::get<double>(right);
    case TokenType::SLASH:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) / std::get<double>(right);
    case TokenType::GREATER:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) > std::get<double>(right);
    case TokenType::GREATER_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) >= std::get<double>(right);
    case TokenType::LESS:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) < std::get<double>(right);
    case TokenType::LESS_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::get<double>(left) <= std::get<double>(right);
    case TokenType::BANG_EQUAL:
        return !isEqual(left, right);
    case TokenType::EQUAL_EQUAL:
        return isEqual(left, right);
    default:
        break;
    }

    throw std::runtime_error("Invalid binary operator.");
}
std::variant<std::monostate, double, bool, std::string> Interpreter::visitGroupingExpr(Grouping *expr)
{
    return evaluate(expr->expression);
}

std::variant<std::monostate, double, bool, std::string> Interpreter::visitLiteralExpr(Literal *expr)
{
    return expr->value;
}

bool Interpreter::isTruthy(const std::variant<std::monostate, double, bool, std::string>& value)
{
    if (std::holds_alternative<std::monostate>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    return true;
}

void Interpreter::checkNumberOperand(const Token& op, const std::variant<std::monostate, double, bool, std::string>& operand)
{
    if (!std::holds_alternative<double>(operand))
        throw std::runtime_error("Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op, const std::variant<std::monostate, double, bool, std::string>& left, const std::variant<std::monostate, double, bool, std::string>& right)
{
    if (!std::holds_alternative<double>(left) || !std::holds_alternative<double>(right))
        throw std::runtime_error("Operands must be numbers.");
}

bool Interpreter::isEqual(const std::variant<std::monostate, double, bool, std::string>& left, const std::variant<std::monostate, double, bool, std::string>& right)
{
    if (left.index() != right.index()) return false;
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return std::get<double>(left) == std::get<double>(right);
    if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) return std::get<bool>(left) == std::get<bool>(right);
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) return std::get<std::string>(left) == std::get<std::string>(right);
    return false;
}

void Interpreter::runtimeError(const Token& token, const std::string& message)
{
    std::cerr << "Runtime Error [line " << token.line << "]: " << message << std::endl;
}
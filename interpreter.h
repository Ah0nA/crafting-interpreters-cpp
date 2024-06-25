#pragma once

#include <Expr.h>
#include <Token.h>
#include <variant>
#include <vector>
#include <stdexcept>

class Interpreter : public Expr::Visitor
{
public:
    std::variant<std::monostate, double, bool, std::string> evaluate(const std::shared_ptr<Expr> &expr);
    void interpret(const std::vector<std::shared_ptr<Expr>> &expressions);

private:
    std::variant<std::monostate, double, bool, std::string> visitBinaryExpr(Binary *expr) override;
    std::variant<std::monostate, double, bool, std::string> visitGroupingExpr(Grouping *expr) override;
    std::variant<std::monostate, double, bool, std::string> visitLiteralExpr(Literal *expr) override;
    std::variant<std::monostate, double, bool, std::string> visitUnaryExpr(Unary *expr) override;

    bool isTruthy(const std::variant<std::monostate, double, bool, std::string> &value);
    bool isEqual(const std::variant<std::monostate, double, bool, std::string> &left, const std::variant<std::monostate, double, bool, std::string> &right);

    void checkNumberOperand(const Token &op, const std::variant<std::monostate, double, bool, std::string> &operand);
    void checkNumberOperands(const Token &op, const std::variant<std::monostate, double, bool, std::string> &left, const std::variant<std::monostate, double, bool, std::string> &right);
    void runtimeError(const Token &op, const std::string &message);

    std::variant<std::monostate, double, bool, std::string> m_value;
};
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
    std::variant<std::monostate, double, bool, std::string> evaluateBinaryExpr(const Binary *expr);
    std::variant<std::monostate, double, bool, std::string> evaluateGroupingExpr(const Grouping *expr); // within parenthesis
    std::variant<std::monostate, double, bool, std::string> evaluateLiteralExpr(const Literal *expr);
    std::variant<std::monostate, double, bool, std::string> evaluateUnaryExpr(const Unary &expr);

    template <typename T>
    double evaluateNumberLiteral(const T &value);

    template <typename T>
    std::string evaluateStringLiteral(const T &value);

    template <typename T>
    bool evaluateBooleanLiteral(const T &value);

    void checkNumberOperand(const Token &op, const std::variant<std::monostate, bool, double, std::string> &operand);
    void checkNumberOperands(const Token&op, const std::variant<std::monostate, double, bool, std::string>& left, const std::variant<std::monostate, double, bool, std::string>& right);
    void runTimeError(const Token& op, const std::string & message);

    std::variant<std::monostate, double, bool, std::string> m_value;
};
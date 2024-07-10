#pragma once

#include <memory>
#include <variant>
#include "Token.h"

class Expr {
public:
    virtual ~Expr() noexcept = default;
    virtual std::variant<std::monostate, double, bool, std::string> accept(class Visitor *visitor) = 0;
};

class Binary : public Expr {
public:
     std::shared_ptr<Expr> left;
     Token op;
     std::shared_ptr<Expr> right;
    Binary( std::shared_ptr<Expr> left,  Token op,  std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitBinaryExpr(this);
    }
    virtual ~Binary() noexcept = default;
};

class Grouping : public Expr {
public:
     std::shared_ptr<Expr> expression;
    Grouping( std::shared_ptr<Expr> expression) : expression(expression) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitGroupingExpr(this);
    }
    virtual ~Grouping() noexcept = default;
};

class Literal : public Expr {
public:
    std::variant<std::monostate, double, bool, std::string> value;
    Literal(std::variant<std::monostate, double, bool, std::string> value) : value(value) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitLiteralExpr(this);
    }
    virtual ~Literal() noexcept = default;
};

class Unary : public Expr {
public:
     Token op;
     std::shared_ptr<Expr> right;
    Unary( Token op,  std::shared_ptr<Expr> right) : op(op), right(right) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitUnaryExpr(this);
    }
    virtual ~Unary() noexcept = default;
};

class Visitor {
public:
    virtual std::variant<std::monostate, double, bool, std::string> visitBinaryExpr(class Binary *stmt) = 0;
    virtual std::variant<std::monostate, double, bool, std::string> visitGroupingExpr(class Grouping *stmt) = 0;
    virtual std::variant<std::monostate, double, bool, std::string> visitLiteralExpr(class Literal *stmt) = 0;
    virtual std::variant<std::monostate, double, bool, std::string> visitUnaryExpr(class Unary *stmt) = 0;
};


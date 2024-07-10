#pragma once

#include <memory>
#include <variant>
#include "Token.h"

#include "Expr.h"

class Stmt {
public:
    virtual ~Stmt() noexcept = default;
    virtual std::variant<std::monostate, double, bool, std::string> accept(class Visitor *visitor) = 0;
};

class Expression : public Stmt {
public:
     std::shared_ptr<Expr> expression;
    Expression( std::shared_ptr<Expr> expression) : expression(expression) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitExpressionStmt(this);
    }
    virtual ~Expression() noexcept = default;
};

class Print : public Stmt {
public:
     std::shared_ptr<Expr> expression;
    Print( std::shared_ptr<Expr> expression) : expression(expression) {}
    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {
        return visitor->visitPrintStmt(this);
    }
    virtual ~Print() noexcept = default;
};

class Visitor {
public:
    virtual std::variant<std::monostate, double, bool, std::string> visitExpressionStmt(class Expression *stmt) = 0;
    virtual std::variant<std::monostate, double, bool, std::string> visitPrintStmt(class Print *stmt) = 0;
};


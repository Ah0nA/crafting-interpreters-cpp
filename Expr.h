/*
expression → literal
 | unary
 | binary
 | grouping ;
literal → NUMBER | STRING | "true" | "false" | "nil" ;
grouping → "(" expression ")" ;
unary → ( "-" | "!" ) expression ;
binary → expression operator expression ;
operator → "==" | "!=" | "<" | "<=" | ">" | ">="
 | "+" | "-" | "*" | "/" ;
*/

#pragma once

#include <memory>
#include <variant>
#include "Token.h"

class Expr
{
public:
    class Visitor
    {
    public:
        virtual std::variant<std::monostate, double, bool, std::string> visitBinaryExpr(class Binary *expr) = 0;
        virtual std::variant<std::monostate, double, bool, std::string> visitGroupingExpr(class Grouping *expr) = 0;
        virtual std::variant<std::monostate, double, bool, std::string> visitLiteralExpr(class Literal *expr) = 0;
        virtual std::variant<std::monostate, double, bool, std::string> visitUnaryExpr(class Unary *expr) = 0;
    };

    virtual std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) = 0;

    virtual ~Expr() = default;
};

class Binary : public Expr
{
public:
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}

    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override
    {
        return visitor->visitBinaryExpr(this);
    }
};

class Grouping : public Expr
{
public:
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expression) : expression(expression) {}

    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override
    {
        return visitor->visitGroupingExpr(this);
    }
};

class Literal : public Expr
{
public:
    std::variant<std::monostate, double, bool, std::string> value;

    Literal(std::variant<std::monostate, double, bool, std::string> value) : value(value) {}

    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override
    {
        return visitor->visitLiteralExpr(this);
    }
};

class Unary : public Expr
{
public:
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right) : op(op), right(right) {}

    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override
    {
        return visitor->visitUnaryExpr(this);
    }
};

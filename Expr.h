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
#include <Tokentype.h>
#include <Token.h>

class Expr
{
public:
    class Visitor
    {
    public:
        virtual void visitBinaryExpr(class Binary *expr) = 0;
        virtual void visitGroupingExpr(class Grouping *expr) = 0;
        virtual void visitLiteralExpr(class Literal *expr) = 0;
        virtual void visitUnaryExpr(class Unary *expr) = 0;
    };

    virtual void accept(Visitor *visitor) = 0;

    virtual ~Expr() = default;
};

class Binary : public Expr
{
public:
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}

    void accept(Visitor *visitor) override
    {
        visitor->visitBinaryExpr(this);
    }
};

class Grouping : public Expr
{
public:
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expression)
        : expression(expression) {}
    void accept(Visitor *visitor) override
    {
        visitor->visitGroupingExpr(this);
    }
};

class Literal : public Expr
{
public:
    std::variant<std::monostate, std::string, double,bool> value;

    Literal(std::variant<std::monostate, std::string, double, bool> value) : value(value) {}

    void accept(Visitor *visitor) override
    {
        visitor->visitLiteralExpr(this);
    }
};

class Unary : public Expr
{
    Token op;
    std::shared_ptr<Expr> right;

    Unary(std::shared_ptr<Expr> right, Token op) : right(right),
                                                   op(op) {}

    void accept(Visitor *visitor) override
    {
        visitor->visitUnaryExpr(this);
    }
};
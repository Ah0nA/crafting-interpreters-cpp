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
};
#pragma once

#include <Tokentype.h>
#include "build/Stmt.h"
#include <Token.h>
#include <Expr.h>
#include <vector>
#include <memory>

class Parser
{
private:
    std::vector<Token> tokens;
    int current = 0;

public:
    Parser(const std::vector<Token> &tokens);

private:
private:
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    bool match(const std::vector<TokenType> &types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    void synchronize();
    std::shared_ptr<Stmt> printStatement();
    std::vector<std::shared_ptr<Stmt>> Parser::parser();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> expressionStatement();
    Token consume(TokenType type, const std::string &message);
    std::string reportError(const Token &token, const std::string &message);
    std::shared_ptr<Expr> comparision();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
};

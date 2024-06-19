#pragma once

#include <Tokentype.h>
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
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    bool match(const std::vector<TokenType> &types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    std::shared_ptr<Expr> comparision();
};

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
    Parser(const std::vector<Token> tokens) : tokens(tokens){}
    
};

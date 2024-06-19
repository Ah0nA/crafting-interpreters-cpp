#include "Parser.h"

// tokens -> AST

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

bool Parser::check(TokenType type){
    if(isAtEnd) return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType> &types){
    for(TokenType type: types){
        if(check(type)){
            advance();
            return true;
        }
    }
    return false;
}

std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

// equality → comparison ( ( "!=" | "==" ) comparison )* ;

std::shared_ptr<Expr> Parser::equality()
{
    auto expr = Parser::comparision();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {

    }
}


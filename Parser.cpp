#include "Parser.h"
#include "lox.h"
#include <stdexcept>

// tokens -> AST

class ParseError : public std::runtime_error
{
public:
    ParseError(const std::string &message) : std::runtime_error(message) {}
};
Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), current(0) {}

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

Token Parser::consume(TokenType type, const std::string &message)
{
    if (check(type))
    {
        advance();
    }
    throw ParseError(reportError(peek(), message));
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().type == TokenType::SEMICOLON)
            return;

        switch (peek().type)
        {
        case TokenType::CLASS:
            /* code */
            break;
        case TokenType::FUN:
            /* code */
            break;
        case TokenType::VAR:
            /* code */
            break;
        case TokenType::FOR:
            /* code */
            break;
        case TokenType::IF:
            /* code */
            break;
        case TokenType::WHILE:
            /* code */
            break;
        case TokenType::PRINT:
            /* code */
            break;
        case TokenType::RETURN:
            return;
            break;

        default:
            break;
        }
        advance();
    }
}

std::string Parser::reportError(const Token &token, const std::string &message)
{
    std::string errorMessage;
    if (token.type == TokenType::END_OF_FILE)
    {
        errorMessage = token.line, "' Error at end'" + message;
    }
    else
    {
        errorMessage = token.line, "' Error at '" + token.lexeme + "' : " + message;
    }
    throw ParseError(errorMessage);
}

bool Parser::check(TokenType type)
{
    if (isAtEnd)
        return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType> &types)
{
    for (TokenType type : types)
    {
        if (check(type))
        {
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
        Token op = previous();
        auto right = comparision();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;

std::shared_ptr<Expr> Parser::comparision()
{
    auto expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
                  TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

// term -> factor (("+" | "-") factor)* ;
std::shared_ptr<Expr> Parser::term()
{
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
// factor -> unary (("*" | "/") unary)* ;

std::shared_ptr<Expr> Parser::factor()
{
    auto expr = unary();

    while (match({TokenType::STAR, TokenType::SLASH}))
    {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary>(previous(), right);
    }
    return primary();
}

/*primary → NUMBER | STRING | "true" | "false" | "nil"
 | "(" expression ")" ;*/

std::shared_ptr<Expr> Parser::primary()
{
    if (match({TokenType::TRUE}))
        return std::make_shared<Literal>(true);
    if (match({TokenType::FALSE}))
        return std::make_shared<Literal>(false);
    if (match({TokenType::NIL}))
        return std::make_shared<Literal>(nullptr);

    if (match({TokenType::NUMBER, TokenType::STRING}))
    {
        return std::make_shared<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN}))
    {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return std::make_shared<Grouping>(expr);
    }
    throw ParseError(reportError(peek(), "Unexpected Token"));
}


std::shared_ptr<Stmt> Parser::statement()
{
    if (match({TokenType::PRINT}))
    {
        return printStatement();
    }
    return expressionStatement();
}

std::vector<std::shared_ptr<Stmt>> Parser::parser()
{
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!isAtEnd())
    {
        statements.push_back(statement());
    }

    return statements;
}
std::shared_ptr<Stmt> Parser::printStatement(){
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value");
    return std::make_shared<Print> (value);

}
std::shared_ptr<Stmt> Parser::expressionStatement(){
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value");
    return std::make_shared<Print> (expr);

}



// std::shared_ptr<Expr> Parser::parse()
// {
//     try
//     {
//         return expression();
//     }
//     catch (ParseError &error)
//     {
//         return nullptr;
//     }
// }
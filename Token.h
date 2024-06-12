#pragma once

#include <Tokentype.h>
#include <variant>
#include <string>
#include <stdexcept>

class Token
{
public:
    TokenType type;
    std::string lexeme;
    std::variant<std::monostate, std::string, double> literal;
    int line;

    Token(TokenType type, const std::string &lexeme, const std::variant<std::monostate, std::string, double> &literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
        if (type == TokenType::NUMBER && !std::holds_alternative<double>(literal))
        {
            throw std::invalid_argument("Invalid literal type for NUMBER token");
        }
        else if ((type == TokenType::STRING || type == TokenType::IDENTIFIER) && !std::holds_alternative<std::string>(literal))
        {
            throw std::invalid_argument("Invalid literal type for STRING/IDENTIFIER token");
        }
        else if (type == TokenType::NIL && !std::holds_alternative<std::monostate>(literal))
        {
            throw std::invalid_argument("Invalid literal type for NIL token");
        }
        else if ((type == TokenType::TRUE || type == TokenType::FALSE) && !std::holds_alternative<std::monostate>(literal))
        {
            throw std::invalid_argument("Invalid literal type for BOOLEAN token");
        }
        else if (type == TokenType::END_OF_FILE && !std::holds_alternative<std::monostate>(literal))
        {
            throw std::invalid_argument("Invalid literal type for END_OF_FILE token");
        }
    }

    std::string toString() const
    {
        std::string literalStr = std::visit([](auto &&arg) -> std::string
                                            {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) return arg;
            else if constexpr (std::is_same_v<T, double>) return std::to_string(arg);
            else return "nil"; }, literal);

        return tokenTypeToString(type) + " " + lexeme + " " + literalStr;
    }

private:
    std::string tokenTypeToString(TokenType type) const
    {
        switch (type)
        {
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::STAR:
            return "STAR";
        case TokenType::BANG:
            return "BANG";
        case TokenType::BANG_EQUAL:
            return "BANG_EQUAL";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case TokenType::LESS:
            return "LESS";
        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::AND:
            return "AND";
        case TokenType::CLASS:
            return "CLASS";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::FUN:
            return "FUN";
        case TokenType::FOR:
            return "FOR";
        case TokenType::IF:
            return "IF";
        case TokenType::NIL:
            return "NIL";
        case TokenType::OR:
            return "OR";
        case TokenType::PRINT:
            return "PRINT";
        case TokenType::RETURN:
            return "RETURN";
        case TokenType::SUPER:
            return "SUPER";
        case TokenType::THIS:
            return "THIS";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::VAR:
            return "VAR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::END_OF_FILE:
            return "END_OF_FILE";
        default:
            return "UNKNOWN";
        }
    }
};
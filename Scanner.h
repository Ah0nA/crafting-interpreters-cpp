#pragma once

#include <Token.h>
#include <Tokentype.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <lox.h>





class Scanner
{
public:
    Scanner(const std::string &source, int startingLine = 1)
        : source(source), start(0), current(0), line(startingLine) {}

    std::vector<Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        tokens.emplace_back(TokenType::END_OF_FILE, "", std::monostate(), line);
        return tokens;
    }

private:
    std::string source;
    std::vector<Token> tokens;
    int start;
    int current;
    int line;

    bool isAtEnd()
    {
        return current >= source.length();
    }

    void Scanner::scanToken()
    {
        char c = advance();
        switch (c)
        {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('*'))
            {
                int nestedDepth = 1;
                while(nestedDepth > 0 && !isAtEnd()){
                    if(peek() == '\n'){
                        line++; 
                        advance();
                    }
                    else if(peek() == '/' && peekNext() == '*'){
                        nestedDepth++;
                        advance();
                        advance();
                    }else if(peek() == '*' && peekNext() == '/'){
                        nestedDepth--;
                        advance();
                        advance();

                    }else{
                        advance();
                    }
                   
                }
                if(nestedDepth > 0){
                     throw "Unterminated block comment" ;
                }
            }
            
      
            else if (match('/'))
            {
                while (peek() != '\n' && !isAtEnd())
                    advance();
            }
            else
            {
                addToken(TokenType::SLASH);
            }
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            string();
            break;

        default:
            if (isdigit(c))
            {
                number();
            }
            else if (isalpha(c))
            {
                identifier();
            }
            else
            {
                std::string errorMessage = "Unexpected character: ";
                errorMessage += c;
                Lox::error(line, errorMessage);
            }

            break;
        }
    }

    char Scanner::advance()
    {
        current++;
        return source[current - 1];
    }

    bool Scanner::match(char expected)
    {
        if (isAtEnd())
            return false;
        if (source[current != expected])
            return false;
        current++;
        return true;
    }

    char peek()
    {
        if (isAtEnd())
            return '\0';
        return source[current];
    }

    void Scanner::string()
    {
        while (peek() != '"' && isAtEnd())
        {
            if (peek() == '\n')
                line++;
            advance();
        }
        if (isAtEnd())
        {
            std::string errorNote = "Unterminated string";
            Lox::error(line, errorNote);
            return;
        }
        advance();

        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
    }

    bool isDigit(char c)
    {
        return c >= 0 && c <= 9;
    }

    void Scanner::number()
    {
        while (isDigit(peek()))
            advance();

        if (peek() == '.' && isDigit(peekNext()))
        {
            advance();

            while (isDigit(peek()))
                advance();
        }
        std::string numberString = source.substr(start, current - start);
        addToken(TokenType::NUMBER, std::stod(numberString));
    }

    bool isAlpha(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
    }

    bool isAlphaNumeric(char c)
    {
        return isDigit || isAlpha;
    }

    void Scanner::identifier()
    {
        while (isAlphaNumeric(peek()))
            advance();
        std::string text = source.substr(start, current - start);
        auto it = keywords.find(text);
        TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
        addToken(type);
    }

    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source[current + 1];
    }

    void Scanner::addToken(TokenType type)
    {
        addToken(type, std::monostate{});
    }

    void Scanner::addToken(TokenType type, std::variant<std::monostate, std::string, double> literal)
    {
        std::string text = source.substr(start, current - start);
        tokens.emplace_back(type, text, literal, line);
    }
    const std::unordered_map<std::string, TokenType> Scanner::keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}};
};
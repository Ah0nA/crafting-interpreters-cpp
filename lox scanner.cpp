#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <Tokentype.h>

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
        }
    }

    char Scanner::advance()
    {
        current++;
        return source[current - 1];
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
};

class Lox
{
public:
    static bool hadError;
    static void report(int line, const std::string &where, const std::string &message);
    static void error(int line, std::string &message);
    static void runFile(const std::string &path);
    static void runprompt();
    static void run(const std::string &source);
    static void main(const std::vector<std::string> &args);

    void Lox::error(int line, std::string &message)
    {
        report(line, " ", message);
    }

    void Lox::main(const std::vector<std::string> &args)
    {
        if (args.size() > 1)
        {
            std::cout << "Usage: jlox [script]";
            exit(64);
        }
        else if (args.size() == 1)
        {
            runFile(args[0]);
            if (hadError)
                exit(65);
        }
        else
        {
            runPrompt;
            hadError = false;
        }
    }

private:
    static void report(int line, const std::string &where, const std::string &message)
    {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError = true;
    }

    void Lox::runFile(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file)
        {
            std::cerr << "Unable to  open file" << path << std::endl;
            exit(1);
        }

        // std::streambuf* fileBuffer = file.rdbuf();
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
    }

    void Lox::runPrompt()
    {
        std::string line;

        while (true)
        {
            std::cout << "> ";

            if (!std::getline(std::cin, line))
                break;
            run(line);
            hadError = false;
        }
    }

    void Lox::run(const std::string &source)
    {

        std::cout << "Running source: " << source << std::endl;
    }
};
bool Lox::hadError = false;

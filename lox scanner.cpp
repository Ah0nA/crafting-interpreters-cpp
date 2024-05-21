#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <Tokentype.h>

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

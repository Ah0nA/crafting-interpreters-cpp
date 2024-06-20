#include "Scanner.h"
//#include "AstPrinter.h"
#include "Parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

bool hadError = false;

void run(const std::string& source){
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
}

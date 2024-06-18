#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Function to generate a single AST node class
void defineType(std::ofstream &writer, const std::string &baseName, const std::string &className, const std::string &fieldList)
{
    writer << "class" << className << " : public " << baseName << " {\n";
    writer << "public:\n";

    //"std::shared_ptr<Expr> left, Token operator, std::shared_ptr<Expr> right"
    std::istringstream stream(fieldList);
    std::string field;

    while (std::getline(stream, field, ','))
    {
        writer << " " << field << ";\n";
    }
    // Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) :
    //  left(left), op(op), right(right) {}
    writer << "    " << className << "(" << fieldList << ") : ";
    std::istringstream streamInit(fieldList);

    bool first = true;
    while (std::getline(streamInit, field, ','))
    {
        std::string name = field.substr(field.find_last_of(' ') + 1);
        if (!first)
            writer << " ,";
        writer << name << "(" << name << ")";
        first = false;
    }
    writer << " {}\n";
    writer << "};\n";
}

void defineAst(const std::string &outputDir, const std::string &baseName, const std::vector<std::string> &types)
{
    std::string path = outputDir + "/" + baseName + ".h";
    std::ofstream writer(path);

    writer << "#pragma once\n\n";
    writer << "include <memory>\n";
    writer << "include <variant>\n\n";

    writer << "class " << baseName << "{\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "() = default;\n";
    writer << "};\n\n";

    //"Binary : std::shared_ptr<Expr> left, Token operator, std::shared_ptr<Expr> right"
    for (const auto &type : types)
    {
        std::string className = type.substr(0, type.find(':'));
        std::string fields = type.substr(type.find(':') + 1);
        defineType(writer, baseName, className, fields);
        writer << "\n";
    }

    writer.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: generate_ast <output directory>\n";
        return 1;
    }

    std::string outputDir = argv[1];

    defineAst(outputDir, "Expr", {"Binary : std::shared_ptr<Expr> left, Token operator, std::shared_ptr<Expr> right", "Grouping : std::shared_ptr<Expr> expression", "Literal : Object value", "Unary : Token operator, std::shared_ptr<Expr> right"

                                 });
}
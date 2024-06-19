#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

/*class Visitor
    {
    public:
        virtual void visitBinaryExpr(class Binary *expr) = 0;*/
void defineVisitor(std::ofstream &writer, const std::string &baseName, const std::vector<std::string> &types)
{
    writer << "class Visitor\n{\n";
    writer << "public:\n";

    for (const auto type : types)
    {
        std::string typeName = type.substr(0, type.find(":"));
        writer << "    virtual std::string visit" << typeName << baseName << "(class " << typeName << " *" << baseName << ") = 0;\n";
    }
    writer << "};\n\n";
}

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

    /*void accept(Visitor *visitor) override
    {
        visitor->visitBinaryExpr(this);
    }*/

    writer << "   std::string accept(Visitor *visitor) const override {\n";
    writer << "      return visitor -> visit" << className << baseName << "(this);";
    writer << "}\n";

    writer << "};\n";
}

void defineAst(const std::string &outputDir, const std::string &baseName, const std::vector<std::string> &types)
{
    std::string path = outputDir + "/" + baseName + ".h";
    std::ofstream writer(path);

    writer << "#pragma once\n\n";
    writer << "#include <memory>\n";
    writer << "#include <variant>\n\n";
    writer << "#include <Tokentype.h>\n";
    writer << "#include <Token.h>\n";

    writer << "class " << baseName << "{\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "() = default;\n";
    writer << "     virtual std::string accept(class Visitor *visitor) const =0;\n";
    writer << "};\n\n";

    //"Binary : std::shared_ptr<Expr> left, Token operator, std::shared_ptr<Expr> right"
    for (const auto &type : types)
    {
        std::string className = type.substr(0, type.find(':'));
        std::string fields = type.substr(type.find(':') + 1);
        defineType(writer, baseName, className, fields);
        writer << "\n";
    }
    defineVisitor(writer, baseName, types);

    writer.close();


// Generate the AstPrinter class
    std::string printerPath = outputDir + "/AstPrinter.h";
    std::ofstream printerWriter(printerPath);

    printerWriter << "#pragma once\n\n";
    printerWriter << "#include <\"" << baseName << ".h\"\n";
    printerWriter << "#include <sstream>\n";
    printerWriter << "#include <memory>";

    printerWriter << "class AstPrinter : public" << baseName << ":: Visitor \n {";
    printerWriter << "    public: \n";

    printerWriter << "    std::string print(const std::shared_ptr<" << baseName << ">& expr) {\n";
    printerWriter << "       return expr->accept(this);\n";
    printerWriter << "    }\n\n";

    for (const auto &type : types)
    {
        std::string typeName = type.substr(0, type.find(':'));
        printerWriter << "     std::string visit" << typeName << baseName << "(class " << typeName << " *expr) override {\n";
        printerWriter << "         std::stringstream ss;\n";
        printerWriter << "         ss << \"(\" << \"" << typeName << "\";\n";
        std::istringstream fieldsStream(type.substr(type.find(':') + 1));
        std::string field;

        while (std::getline(fieldsStream, field, ','))
        {
            std::string fieldName = field.substr(field.find_last_of(' ') + 1);
            printerWriter << "         ss << \" \" << expr->" << fieldName << ";\n";
        }
        printerWriter << "        ss << \")\";\n";
        printerWriter << "        return ss.str();\n";
        printerWriter << "    }\n\n";
    }
    printerWriter << "};\n";

    printerWriter.close();
}

//Reverse Polish Notation Class


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

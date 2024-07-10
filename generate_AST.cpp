#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>

class Token {
public:
    std::string lexeme;
};

void defineVisitor(std::ofstream &writer, const std::string &baseName, const std::vector<std::string> &types);
void defineType(std::ofstream &writer, const std::string &baseName, const std::string &className, const std::string &fieldList);
void defineAst(const std::string &outputDir, const std::string &baseName, const std::vector<std::string> &types);

void defineVisitor(std::ofstream &writer, const std::string &baseName, const std::vector<std::string> &types) {
    writer << "class Visitor {\n";
    writer << "public:\n";
    for (const auto &type : types) {
        std::string typeName = type.substr(0, type.find(':'));
        writer << "    virtual std::variant<std::monostate, double, bool, std::string> visit" << typeName << baseName << "(class " << typeName << " *stmt) = 0;\n";
    }
    writer << "};\n\n";
}

void defineType(std::ofstream &writer, const std::string &baseName, const std::string &className, const std::string &fieldList) {
    writer << "class " << className << " : public " << baseName << " {\n";
    writer << "public:\n";
    std::istringstream stream(fieldList);
    std::string field;
    std::vector<std::string> fields;
    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
        writer << "    " << field << ";\n";
    }
    writer << "    " << className << "(";
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) writer << ", ";
        writer << fields[i];
    }
    writer << ") : ";
    for (size_t i = 0; i < fields.size(); ++i) {
        std::string name = fields[i].substr(fields[i].find_last_of(' ') + 1);
        if (i > 0) writer << ", ";
        writer << name << "(" << name << ")";
    }
    writer << " {}\n";
    writer << "    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {\n";
    writer << "        return visitor->visit" << className << baseName << "(this);\n";
    writer << "    }\n";
    writer << "    virtual ~" << className << "() noexcept = default;\n";
    writer << "};\n";
}

void defineLiteralType(std::ofstream &writer, const std::string &baseName) {
    writer << "class Literal : public " << baseName << " {\n";
    writer << "public:\n";
    writer << "    std::variant<std::monostate, double, bool, std::string> value;\n";
    writer << "    Literal(std::variant<std::monostate, double, bool, std::string> value) : value(value) {}\n";
    writer << "    std::variant<std::monostate, double, bool, std::string> accept(Visitor *visitor) override {\n";
    writer << "        return visitor->visitLiteral" << baseName << "(this);\n";
    writer << "    }\n";
    writer << "    virtual ~Literal() noexcept = default;\n";
    writer << "};\n";
}

void defineAst(const std::string &outputDir, const std::string &baseName, const std::vector<std::string> &types) {
    std::filesystem::create_directories(outputDir);

    std::string path = outputDir + "/" + baseName + ".h";
    std::ofstream writer(path);

    writer << "#pragma once\n\n";
    writer << "#include <memory>\n";
    writer << "#include <variant>\n";
    writer << "#include \"Token.h\"\n\n";

    if (baseName == "Stmt") {
        writer << "#include \"Expr.h\"\n\n";
    }

    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "() noexcept = default;\n";
    writer << "    virtual std::variant<std::monostate, double, bool, std::string> accept(class Visitor *visitor) = 0;\n";
    writer << "};\n\n";

    for (const auto &type : types) {
        std::string className = type.substr(0, type.find(':'));
        std::string fields = type.substr(type.find(':') + 1);
        if (className == "Literal") {
            defineLiteralType(writer, baseName);
        } else {
            defineType(writer, baseName, className, fields);
        }
        writer << "\n";
    }

    defineVisitor(writer, baseName, types);

    writer.close();

    std::string printerPath = outputDir + "/AstPrinter.h";
    std::ofstream printerWriter(printerPath);

    printerWriter << "#pragma once\n\n";
    printerWriter << "#include \"" << baseName << ".h\"\n";
    printerWriter << "#include <sstream>\n";
    printerWriter << "#include <memory>\n\n";

    printerWriter << "class AstPrinter : public Visitor {\n";
    printerWriter << "public:\n";

    printerWriter << "    std::string print(const std::shared_ptr<" << baseName << ">& stmt) {\n";
    printerWriter << "        auto result = stmt->accept(this);\n";
    printerWriter << "        if (std::holds_alternative<std::string>(result)) {\n";
    printerWriter << "            return std::get<std::string>(result);\n";
    printerWriter << "        }\n";
    printerWriter << "        return \"\";\n";
    printerWriter << "    }\n\n";

    for (const auto &type : types) {
        std::string typeName = type.substr(0, type.find(':'));
        printerWriter << "    std::variant<std::monostate, double, bool, std::string> visit" << typeName << baseName << "(class " << typeName << " *stmt) override {\n";
        printerWriter << "        std::stringstream ss;\n";
        printerWriter << "        ss << \"(" << typeName << " \";\n";

        std::istringstream fieldsStream(type.substr(type.find(':') + 1));
        std::string field;

        while (std::getline(fieldsStream, field, ',')) {
            std::string fieldName = field.substr(field.find_last_of(' ') + 1);
            printerWriter << "        ss << \"" << fieldName << "=\" << stmt->" << fieldName << " << \" \";\n";
        }

        printerWriter << "        ss << \")\";\n";
        printerWriter << "        return ss.str();\n";
        printerWriter << "    }\n\n";
    }

    printerWriter << "};\n";
    printerWriter.close();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output directory>\n";
        return 1;
    }

    std::string outputDir = argv[1];

    defineAst(outputDir, "Expr", {
        "Binary: std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
        "Grouping: std::shared_ptr<Expr> expression",
        "Literal: std::variant<std::monostate, double, bool, std::string> value",
        "Unary: Token op, std::shared_ptr<Expr> right"
    });

    defineAst(outputDir, "Stmt", {
        "Expression: std::shared_ptr<Expr> expression",
        "Print: std::shared_ptr<Expr> expression"
    });

    return 0;
}

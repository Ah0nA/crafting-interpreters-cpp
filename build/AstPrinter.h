#pragma once

#include "Stmt.h"
#include <sstream>
#include <memory>

class AstPrinter : public Visitor {
public:
    std::string print(const std::shared_ptr<Stmt>& stmt) {
        auto result = stmt->accept(this);
        if (std::holds_alternative<std::string>(result)) {
            return std::get<std::string>(result);
        }
        return "";
    }

    std::variant<std::monostate, double, bool, std::string> visitExpressionStmt(class Expression *stmt) override {
        std::stringstream ss;
        ss << "(Expression ";
        ss << "expression=" << stmt->expression << " ";
        ss << ")";
        return ss.str();
    }

    std::variant<std::monostate, double, bool, std::string> visitPrintStmt(class Print *stmt) override {
        std::stringstream ss;
        ss << "(Print ";
        ss << "expression=" << stmt->expression << " ";
        ss << ")";
        return ss.str();
    }

};

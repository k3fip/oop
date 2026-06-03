#pragma once

#include "Expressions.h"
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cmath>

enum class ErrorCode {
    Ok,
    NameAlreadyExists,
    NameDoesNotExist,
    IsFunctionName
};

struct NamedValue {
    std::string name;
    double value;
};

struct Variable {
    double value;
    bool defined;

    Variable() : value(std::numeric_limits<double>::quiet_NaN()), defined(false) {}
};

struct Function {
    std::string name;
    ExprUniquePtr body;

    Function(std::string n, ExprUniquePtr b)
        : name(std::move(n)), body(std::move(b)) {
    }
};

class ExecutionContext {

private:
    std::map<std::string, std::unique_ptr<Variable>> m_variables;
    std::map<std::string, std::unique_ptr<Function>> m_functions;

public:
    ExecutionContext() = default;

    ExecutionContext(const ExecutionContext&) = delete;
    ExecutionContext& operator=(const ExecutionContext&) = delete;

    ExecutionContext(ExecutionContext&&) = default;
    ExecutionContext& operator=(ExecutionContext&&) = default;

    ErrorCode DeclareVariable(const std::string& name) {
        if (m_variables.count(name) || m_functions.count(name)) {
            return ErrorCode::NameAlreadyExists;
        }

        auto var = std::make_unique<Variable>();
        m_variables[name] = std::move(var);
        return ErrorCode::Ok;
    }

    ErrorCode SetVariable(const std::string& name, double value) {
        if (m_functions.count(name)) {
            return ErrorCode::IsFunctionName;
        }

        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            auto var = std::make_unique<Variable>();
            var->value = value;
            var->defined = true;
            m_variables[name] = std::move(var);
        }
        else {
            it->second->value = value;
            it->second->defined = true;
        }

        return ErrorCode::Ok;
    }

    ErrorCode AddFunction(const std::string& name, ExprUniquePtr body) {
        if (m_variables.count(name) || m_functions.count(name)) {
            return ErrorCode::NameAlreadyExists;
        }

        auto fn = std::make_unique<Function>(name, std::move(body));
        m_functions[name] = std::move(fn);
        return ErrorCode::Ok;
    }

    double GetValue(const std::string& name) const {
        auto varIt = m_variables.find(name);
        if (varIt != m_variables.end()) {
            return varIt->second->value;
        }

        auto fnIt = m_functions.find(name);
        if (fnIt != m_functions.end()) {
            return fnIt->second->body->Evaluate(*this);
        }

        return std::numeric_limits<double>::quiet_NaN();
    }

    bool Contains(const std::string& name) const {
        return m_variables.count(name) || m_functions.count(name);
    }

    std::vector<NamedValue> GetVariables() const {
        std::vector<NamedValue> result;
        for (const auto& pair : m_variables) {
            result.push_back({ pair.first, pair.second->value});
        }
        return result;
    }

    std::vector<NamedValue> GetFunctions() const {
        std::vector<NamedValue> result;
        for (const auto& pair : m_functions) {
            double val = pair.second->body->Evaluate(*this);
            result.push_back({ pair.first, val });
        }
        return result;
    }
};

// Реализация VariableExpression::Evaluate после определения ExecutionContext
inline double VariableExpression::Evaluate(const ExecutionContext& ctx) const {
    return ctx.GetValue(m_name);
}

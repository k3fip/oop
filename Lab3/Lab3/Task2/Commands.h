#pragma once

#include "ExecutionContext.h"
#include <memory>
#include <string>
#include <functional>
#include <variant>
#include <sstream>
#include <iomanip>
#include <cmath>

using OutputCallback = std::function<void(const std::string&)>;


inline std::string FormatValue(double value) {
    if (std::isnan(value)) {
        return "nan";
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}


class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void Execute(ExecutionContext& ctx, const OutputCallback& output) = 0;
};

using CommandUniquePtr = std::unique_ptr<ICommand>;

class VarCommand : public ICommand {
private:
    std::string m_name;

public:
    explicit VarCommand(std::string name) : m_name(std::move(name)) {}

    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        auto err = ctx.DeclareVariable(m_name);
        if (err == ErrorCode::NameAlreadyExists) {
            output("Name already exists");
        }
    }
};

class LetCommand : public ICommand {
private:
    std::string m_targetName;
    
    std::variant<double, std::string> m_source;

public:
    LetCommand(std::string target, double value)
        : m_targetName(std::move(target)), m_source(value) {
    }

    LetCommand(std::string target, std::string sourceName)
        : m_targetName(std::move(target)), m_source(std::move(sourceName)) {
    }

    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        double value;

        if (std::holds_alternative<double>(m_source)) {
            value = std::get<double>(m_source);
        }
        else {
            const std::string& sourceName = std::get<std::string>(m_source);
            if (!ctx.Contains(sourceName)) {
                output("Name does not exist");
                return;
            }
            value = ctx.GetValue(sourceName);
        }

        auto err = ctx.SetVariable(m_targetName, value);
        if (err == ErrorCode::IsFunctionName) {
            output("Invalid usage");
        }
    }

};

class FnCommand : public ICommand {
private:
    std::string m_name;
    ExprUniquePtr m_body;

public:
    FnCommand(std::string name, ExprUniquePtr body)
        : m_name(std::move(name)), m_body(std::move(body)) {
    }

    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        auto err = ctx.AddFunction(m_name, std::move(m_body));

        if (err == ErrorCode::NameAlreadyExists) {
            output("Name already exists");
        }
    }
};

class PrintCommand : public ICommand {
private:
    std::string m_name;

public:
    explicit PrintCommand(std::string name) : m_name(std::move(name)) {}

    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        if (!ctx.Contains(m_name)) {
            output("Name does not exist");
            return;
        }

        double value = ctx.GetValue(m_name);
        output(FormatValue(value));
    }
};

class PrintVarsCommand : public ICommand {
public:
    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        auto vars = ctx.GetVariables();
        for (const auto& [name, value] : vars) {
            output(name + ":" + FormatValue(value));
        }
    }
};

class PrintFnsCommand : public ICommand {
public:
    void Execute(ExecutionContext& ctx, const OutputCallback& output) override {
        auto fns = ctx.GetFunctions();
        for (const auto& [name, value] : fns) {
            output(name + ":" + FormatValue(value));
        }
    }
};
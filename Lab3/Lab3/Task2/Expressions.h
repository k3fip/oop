#pragma once

#include <memory>
#include <string>
#include <cmath>
#include <limits>
#include <stdexcept>

class ExecutionContext;

class Expression {
public:
    virtual ~Expression() = default;

    virtual double Evaluate(const ExecutionContext& ctx) const = 0;

    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;

    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;

protected:
    Expression() = default;
};

using ExprUniquePtr = std::unique_ptr<Expression>;

class NumberExpression : public Expression {
private:
    double m_value;

public:
    explicit NumberExpression(double value) : m_value(value) {}

    double Evaluate(const ExecutionContext& ctx) const override {
        return m_value;
    }
};

class VariableExpression : public Expression {
private:
    std::string m_name;

public:
    explicit VariableExpression(std::string name) : m_name(std::move(name)) {}

    double Evaluate(const ExecutionContext&) const override;
};

class BinaryExpression : public Expression {
private:
    ExprUniquePtr m_left;
    ExprUniquePtr m_right;
    char m_operation;
    
public:
    BinaryExpression(ExprUniquePtr left, ExprUniquePtr right, char operation)
        : m_left(std::move(left))
        , m_right(std::move(right))
        , m_operation(operation) {
        if (!IsValidOperation(operation)) {
            throw std::invalid_argument("Invalid binary operation: " + std::string(1, operation));
        }
    }

    BinaryExpression(BinaryExpression&& other) noexcept
        : m_left(std::move(other.m_left))
        , m_right(std::move(other.m_right))
        , m_operation(other.m_operation) {
    }

    BinaryExpression& operator=(BinaryExpression&& other) noexcept {
        if (this != &other) {
            m_left = std::move(other.m_left);
            m_right = std::move(other.m_right);
            m_operation = other.m_operation;
        }
        return *this;
    }

    bool IsValidOperation(char op) {
        return op == '+' || op == '-' || op == '*' || op == '/';
    }

    double Evaluate(const ExecutionContext& ctx) const override {
        double leftVal = m_left->Evaluate(ctx);
        double rightVal = m_right->Evaluate(ctx);

        if (std::isnan(leftVal) || std::isnan(rightVal)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        switch (m_operation) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/':
            if (std::abs(rightVal) < std::numeric_limits<double>::epsilon()) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            return leftVal / rightVal;
        default:
            return std::numeric_limits<double>::quiet_NaN();
        }
    }
};
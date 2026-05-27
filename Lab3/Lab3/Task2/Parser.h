#pragma once

#include "Commands.h"
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <memory>

enum class Status { Ok, Error };

class Parser {
public:
    struct ParseResult {
        Status status;
        CommandUniquePtr command;
        std::string errorMessage;

        static ParseResult Success(CommandUniquePtr cmd) {
            return { Status::Ok, std::move(cmd), "" };
        }

        static ParseResult Error(const std::string& msg) {
            return { Status::Error, nullptr, msg };
        }
    };

    static ParseResult Parse(const std::string& line) {
        auto tokens = Tokenize(line);

        if (tokens.empty()) {
            return ParseResult::Error("Unknown command");
        }

        const std::string& command = tokens[0];

        if (command == "var") return ParseVar(tokens);
        if (command == "let") return ParseLet(tokens);
        if (command == "fn") return ParseFn(tokens);
        if (command == "print") return ParsePrint(tokens);
        if (command == "printvars") return ParsePrintVars(tokens);
        if (command == "printfns") return ParsePrintFns(tokens);

        return ParseResult::Error("Unknown command");
    }

private:
    static std::vector<std::string> Tokenize(const std::string& line) {
        std::vector<std::string> tokens;
        std::string current;

        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];
           
            if (ch == ' ') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            }
            else if (ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back(std::string(1, ch));
            }
            else {
                current += ch;
            }
        }

        if (!current.empty()) {
            tokens.push_back(current);
        }

        return tokens;
    }

    static bool IsValidIdentifier(const std::string& name) {
        if (name.empty()) return false;
        if (std::isdigit(static_cast<unsigned char>(name[0]))) return false;

        return std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
            });
    }

    static ParseResult ParseVar(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2 || !IsValidIdentifier(tokens[1])) {
            return ParseResult::Error("Invalid usage");
        }

        return ParseResult::Success(std::make_unique<VarCommand>(tokens[1]));
    }

    static ParseResult ParseLet(const std::vector<std::string>& tokens) {
        if (tokens.size() != 4) {
            return ParseResult::Error("Invalid usage");
        }

        if (!IsValidIdentifier(tokens[1]) || tokens[2] != "=") {
            return ParseResult::Error("Invalid usage");
        }

        const std::string& rightSide = tokens[3];

        try {
            size_t pos = 0;
            double value = std::stod(rightSide, &pos);
            if (pos == rightSide.size()) {
                return ParseResult::Success(
                    std::make_unique<LetCommand>(tokens[1], value)
                );
            }
        }
        catch (...) {
            
        }

        if (!IsValidIdentifier(rightSide)) return ParseResult::Error("Invalid usage");

        return ParseResult::Success(
            std::make_unique<LetCommand>(tokens[1], rightSide)
        );
    }

    static ParseResult ParseFn(const std::vector<std::string>& tokens) {
        if (tokens.size() < 4) {
            return ParseResult::Error("Invalid usage");
        }

        if (!IsValidIdentifier(tokens[1]) || tokens[2] != "=") {
            return ParseResult::Error("Invalid usage");
        }

        std::vector<std::string> exprTokens(tokens.begin() + 3, tokens.end());
        auto expr = ParseExpression(exprTokens);

        if (!expr) {
            return ParseResult::Error("Invalid usage");
        }

        return ParseResult::Success(
            std::make_unique<FnCommand>(tokens[1], std::move(expr))
        );
    }

    static ParseResult ParsePrint(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2 || !IsValidIdentifier(tokens[1])) {
            return ParseResult::Error("Invalid usage");
        }

        return ParseResult::Success(std::make_unique<PrintCommand>(tokens[1]));
    }

    static ParseResult ParsePrintVars(const std::vector<std::string>& tokens) {
        if (tokens.size() != 1) {
            return ParseResult::Error("Invalid usage");
        }

        return ParseResult::Success(std::make_unique<PrintVarsCommand>());
    }

    static ParseResult ParsePrintFns(const std::vector<std::string>& tokens) {
        if (tokens.size() != 1) {
            return ParseResult::Error("Invalid usage");
        }

        return ParseResult::Success(std::make_unique<PrintFnsCommand>());
    }

    static ExprUniquePtr ParseExpression(const std::vector<std::string>& tokens) {
        if (tokens.empty()) return nullptr;

        if (tokens.size() == 1) {
            const std::string& token = tokens[0];

            if (IsValidIdentifier(token)) {
                return std::make_unique<VariableExpression>(token);
            }

            return nullptr;
        }

        if (tokens.size() == 3) {
            if (tokens[1] == "+" || tokens[1] == "-" ||
                tokens[1] == "*" || tokens[1] == "/") {

                std::vector<std::string> leftToken(tokens.begin(), tokens.begin() + 1);
                std::vector<std::string> rightToken(tokens.begin() + 2, tokens.end());

                auto left = ParseExpression(leftToken);
                auto right = ParseExpression(rightToken);

                if (left && right) {
                    return std::make_unique<BinaryExpression>(
                        std::move(left), std::move(right), tokens[1][0]
                    );
                }
            }
        }

        return nullptr;
    }
};
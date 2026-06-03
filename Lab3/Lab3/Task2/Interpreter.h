#pragma once

#include "ExecutionContext.h"
#include "Parser.h"
#include <istream>
#include <string>

class Interpreter {
private:
    ExecutionContext m_context;
    OutputCallback m_output;

public:
    explicit Interpreter(OutputCallback output)
        : m_output(std::move(output)) {
    }

    void Run(std::istream& input) {
        std::string line;

        while (std::getline(input, line)) {
            if (line.empty()) continue;

            auto parseResult = Parser::Parse(line);

            if (parseResult.status == Status::Error) {
                m_output(parseResult.errorMessage);
                continue;
            }

            parseResult.command->Execute(m_context, m_output);
        }
    }
};
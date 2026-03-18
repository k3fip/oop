#pragma once

#include <string>
#include "Car.h"

enum class CommandType
{
    INFO,
    ENGINE_ON,
    ENGINE_OFF,
    SET_GEAR,
    SET_SPEED,
    UNKNOWN
};

struct ParsedCommand
{
    CommandType type;
    std::string argument;
    bool hasArgument;
};

bool IsInteger(const std::string& str);
void PrintInfo(const Car& car);
ParsedCommand ParseCommand(const std::string& input);
void HandleEngineOn(Car& car);
void HandleEngineOff(Car& car);
void HandleSetGear(Car& car, const std::string& arg);
void HandleSetSpeed(Car& car, const std::string& arg);
void ProcessCommand(Car& car, const std::string& commandLine);
void SimulateCar();
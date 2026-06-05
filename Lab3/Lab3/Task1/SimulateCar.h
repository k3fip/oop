#pragma once

#include "Car.h"
#include <string>
#include <map>

static constexpr const char* CAR_CONTROL_STARTED = "Car control started. Enter commands (Info, EngineOn, EngineOff, SetGear, SetSpeed):";
static constexpr const char* ENGINE_ON_FAILED = "Failed to turn on engine. Ensure gear is neutral and speed is 0";
static constexpr const char* ENGINE_OFF_FAILED = "Failed to turn off engine. Ensure gear is neutral and speed is 0";
static constexpr const char* SET_GEAR_FAILED = "Failed to set gear. Check speed limits or engine state";
static constexpr const char* SET_SPEED_FAILED = "Failed to set speed. Check gear limits or engine state";
static constexpr const char* INVALID_ARGUMENT = "Invalid command argument";
static constexpr const char* UNKNOWN_COMMAND = "Unknown command";
static constexpr const char* ENGINE_ON = "on";
static constexpr const char* ENGINE_OFF = "off";
static constexpr const char* ENGINE_LABEL = "Engine: ";
static constexpr const char* DIRECTION_LABEL = "Direction: ";
static constexpr const char* SPEED_LABEL = "Speed: ";
static constexpr const char* GEAR_LABEL = "Gear: ";

static constexpr const char* INFO_CMD = "info";
static constexpr const char* ENGINE_ON_CMD = "engineon";
static constexpr const char* ENGINE_OFF_CMD = "engineoff";
static constexpr const char* SET_GEAR_CMD = "setgear";
static constexpr const char* SET_SPEED_CMD = "setspeed";

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
    CommandType type = CommandType::UNKNOWN;
    std::string argument;
    bool hasArgument = false;
};

inline static const std::map<std::string, CommandType> COMMAND_MAP = {
        {INFO_CMD, CommandType::INFO},
        {ENGINE_ON_CMD, CommandType::ENGINE_ON},
        {ENGINE_OFF_CMD, CommandType::ENGINE_OFF},
        {SET_GEAR_CMD, CommandType::SET_GEAR},
        {SET_SPEED_CMD, CommandType::SET_SPEED}
};

class CommandHandler
{
public:
    static void SimulateCar();
    static void PrintInfo(const class Car& car);
    static ParsedCommand ParseCommand(const std::string& input);
    static void HandleEngineOn(class Car& car);
    static void HandleEngineOff(class Car& car);
    static void HandleSetGear(class Car& car, const std::string& arg);
    static void HandleSetSpeed(class Car& car, const std::string& arg);
    static void ProcessCommand(class Car& car, const std::string& commandLine);
};
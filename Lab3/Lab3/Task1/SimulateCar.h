#pragma once

#include "Car.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <map>

class CommandHandler
{
public:
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

    static void SimulateCar()
    {
        Car car;
        std::string commandLine;

        std::cout << CAR_CONTROL_STARTED << std::endl;

        while (std::getline(std::cin, commandLine))
        {
            if (commandLine.empty()) continue;
            ProcessCommand(car, commandLine);
        }
    }

    inline static const std::map<std::string, CommandType> COMMAND_MAP = {
        {INFO_CMD, CommandType::INFO},
        {ENGINE_ON_CMD, CommandType::ENGINE_ON},
        {ENGINE_OFF_CMD, CommandType::ENGINE_OFF},
        {SET_GEAR_CMD, CommandType::SET_GEAR},
        {SET_SPEED_CMD, CommandType::SET_SPEED}
    };

    static bool IsInteger(const std::string& str)
    {
        if (str.empty()) return false;

        size_t start = 0;
        if (str[0] == '-')
        {
            if (str.length() == 1) return false;
            start = 1;
        }

        for (size_t i = start; i < str.length(); ++i)
        {
            if (!isdigit(str[i])) return false;
        }
        return true;
    }

    static void PrintInfo(const Car& car)
    {
        std::cout << ENGINE_LABEL << (car.GetEngine() ? ENGINE_ON : ENGINE_OFF) << std::endl;
        std::cout << DIRECTION_LABEL << car.GetDirection() << std::endl;
        std::cout << SPEED_LABEL << car.GetSpeed() << std::endl;
        std::cout << GEAR_LABEL << car.GetGear() << std::endl;
    }

    static ParsedCommand ParseCommand(const std::string& input)
    {
        std::istringstream iss(input);
        std::string action;
        iss >> action;

        if (action.empty())
        {
            return { CommandType::UNKNOWN, "", false };
        }

        std::string actionLower = action;
        std::transform(actionLower.begin(), actionLower.end(), actionLower.begin(), ::tolower);

        auto it = COMMAND_MAP.find(actionLower);
        if (it != COMMAND_MAP.end())
        {
            ParsedCommand result;
            result.type = it->second;

            if (it->second == CommandType::SET_GEAR || it->second == CommandType::SET_SPEED)
            {
                std::string arg;
                iss >> arg;
                result.hasArgument = !arg.empty();
                result.argument = arg;
            }
            else
            {
                result.hasArgument = false;
            }

            return result;
        }

        return { CommandType::UNKNOWN, "", false };
    }

    static void HandleEngineOn(Car& car)
    {
        if (!car.TurnOnEngine())
        {
            std::cout << ENGINE_ON_FAILED << std::endl;
        }
    }

    static void HandleEngineOff(Car& car)
    {
        if (!car.TurnOffEngine())
        {
            std::cout << ENGINE_OFF_FAILED << std::endl;
        }
    }

    static void HandleSetGear(Car& car, const std::string& arg)
    {
        if (arg.empty() || !IsInteger(arg))
        {
            std::cout << INVALID_ARGUMENT << std::endl;
            return;
        }

        int gear = std::stoi(arg);

        if (!car.SetGear(gear))
        {
            std::cout << SET_GEAR_FAILED << std::endl;
        }
    }

    static void HandleSetSpeed(Car& car, const std::string& arg)
    {
        if (arg.empty() || !IsInteger(arg))
        {
            std::cout << INVALID_ARGUMENT << std::endl;
            return;
        }

        int speed = std::stoi(arg);

        if (!car.SetSpeed(speed))
        {
            std::cout << SET_SPEED_FAILED << std::endl;
        }
    }

    static void ProcessCommand(Car& car, const std::string& commandLine)
    {
        ParsedCommand cmd = ParseCommand(commandLine);

        switch (cmd.type)
        {
        case CommandType::INFO:
            PrintInfo(car);
            break;

        case CommandType::ENGINE_ON:
            HandleEngineOn(car);
            break;

        case CommandType::ENGINE_OFF:
            HandleEngineOff(car);
            break;

        case CommandType::SET_GEAR:
            if (!cmd.hasArgument)
            {
                std::cout << INVALID_ARGUMENT << std::endl;
            }
            else
            {
                HandleSetGear(car, cmd.argument);
            }
            break;

        case CommandType::SET_SPEED:
            if (!cmd.hasArgument)
            {
                std::cout << INVALID_ARGUMENT << std::endl;
            }
            else
            {
                HandleSetSpeed(car, cmd.argument);
            }
            break;

        case CommandType::UNKNOWN:
        default:
            std::cout << UNKNOWN_COMMAND << std::endl;
            break;
        }
    }
};
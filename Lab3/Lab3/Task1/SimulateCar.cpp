#include "Car.h"
#include "SimulateCar.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <map>
#include <functional>

//не надо указывать мапы, только классы
const std::map<std::string, CommandType> COMMAND_MAP = {
        {"info", CommandType::INFO},
        {"engineon", CommandType::ENGINE_ON},
        {"engineoff", CommandType::ENGINE_OFF},
        {"setgear", CommandType::SET_GEAR},
        {"setspeed", CommandType::SET_SPEED}
};

bool IsInteger(const std::string& str)
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

void PrintInfo(const Car& car)
{
    std::cout << "Engine: " << (car.GetEngine() ? "on" : "off") << std::endl;
    std::cout << "Direction: " << car.GetDirection() << std::endl;
    std::cout << "Speed: " << car.GetSpeed() << std::endl;
    std::cout << "Gear: " << car.GetGear() << std::endl;
}

ParsedCommand ParseCommand(const std::string& input)
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

void HandleEngineOn(Car& car)
{
    !car.TurnOnEngine();
}

void HandleEngineOff(Car& car)
{
    car.TurnOffEngine();
}

void HandleSetGear(Car& car, const std::string& arg)
{
    if (arg.empty() || !IsInteger(arg))
    {
        std::cout << "Invalid command argument" << std::endl;
        return;
    }

    int gear = std::stoi(arg);

    car.SetGear(gear);
}

void HandleSetSpeed(Car& car, const std::string& arg)
{
    if (arg.empty() || !IsInteger(arg))
    {
        std::cout << "Invalid command argument" << std::endl;
        return;
    }

    int speed = std::stoi(arg);

    car.SetSpeed(speed);
}

void ProcessCommand(Car& car, const std::string& commandLine)
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
            std::cout << "Invalid command argument" << std::endl;
        }
        else
        {
            HandleSetGear(car, cmd.argument);
        }
        break;

    case CommandType::SET_SPEED:
        if (!cmd.hasArgument)
        {
            std::cout << "Invalid command argument" << std::endl;
        }
        else
        {
            HandleSetSpeed(car, cmd.argument);
        }
        break;

    case CommandType::UNKNOWN:
    default:
        std::cout << "Unknown command" << std::endl;
        break;
    }
}

void SimulateCar()
{
    Car car;
    std::string commandLine;

    std::cout << "Car control started. Enter commands (Info, EngineOn, EngineOff, SetGear, SetSpeed):" << std::endl;

    while (std::getline(std::cin, commandLine))
    {
        if (commandLine.empty()) continue;

        ProcessCommand(car, commandLine);
    }
}

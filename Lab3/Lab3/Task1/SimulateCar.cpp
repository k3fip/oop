#include "SimulateCar.h"
#include "Car.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

void CommandHandler::SimulateCar()
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

void CommandHandler::PrintInfo(const Car& car)
{
    std::cout << ENGINE_LABEL << (car.GetEngine() ? ENGINE_ON : ENGINE_OFF) << std::endl;
    std::cout << DIRECTION_LABEL << car.GetDirection() << std::endl;
    std::cout << SPEED_LABEL << car.GetSpeed() << std::endl;
    std::cout << GEAR_LABEL << car.GetGear() << std::endl;
}

ParsedCommand CommandHandler::ParseCommand(const std::string& input)
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

void CommandHandler::HandleEngineOn(Car& car)
{
    if (!car.TurnOnEngine())
    {
        std::cout << ENGINE_ON_FAILED << std::endl;
    }
}

void CommandHandler::HandleEngineOff(Car& car)
{
    if (!car.TurnOffEngine())
    {
        std::cout << ENGINE_OFF_FAILED << std::endl;
    }
}

void CommandHandler::HandleSetGear(Car& car, const std::string& arg)
{
    try
    {
        int gear = std::stoi(arg);

        if (!car.SetGear(gear))
        {
            std::cout << SET_GEAR_FAILED << std::endl;
        }
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_ARGUMENT << std::endl;
    }
}

void CommandHandler::HandleSetSpeed(Car& car, const std::string& arg)
{
    try
    {
        int speed = std::stoi(arg);

        if (!car.SetSpeed(speed))
        {
            std::cout << SET_SPEED_FAILED << std::endl;
        }
    }
    catch (const std::exception&)
    {
        std::cout << INVALID_ARGUMENT << std::endl;
    }
}

void CommandHandler::ProcessCommand(Car& car, const std::string& commandLine)
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
        HandleSetGear(car, cmd.argument);
        break;

    case CommandType::SET_SPEED:
        HandleSetSpeed(car, cmd.argument);
        break;

    case CommandType::UNKNOWN:
    default:
        std::cout << UNKNOWN_COMMAND << std::endl;
        break;
    }
}
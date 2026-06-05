#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "../Task1/Car.h"
#include "../Task1/SimulateCar.h"

class CaptureOutput
{
public:
    CaptureOutput() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CaptureOutput() { std::cout.rdbuf(old); }
    std::string getOutput() { return buffer.str(); }
    void clear() { buffer.str(""); buffer.clear(); }

private:
    std::stringstream buffer;
    std::streambuf* old;
};

TEST_CASE("Начальное состояние автомобиля")
{
    Car car;
    REQUIRE_FALSE(car.GetEngine());
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetGear() == 0);
    REQUIRE(car.GetDirection() == "standing still");
}

TEST_CASE("Управление двигателем через CommandHandler")
{
    SECTION("Успешное включение двигателя")
    {
        Car car;
        CaptureOutput capture;
        CommandHandler::HandleEngineOn(car);
        REQUIRE(car.GetEngine());
        REQUIRE(capture.getOutput().empty());
    }

    SECTION("Успешное выключение двигателя")
    {
        Car car;
        car.TurnOnEngine();
        CaptureOutput capture;
        CommandHandler::HandleEngineOff(car);
        REQUIRE_FALSE(car.GetEngine());
        REQUIRE(capture.getOutput().empty());
    }

    SECTION("Ошибка включения при включенной передаче")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        car.TurnOffEngine();

        CaptureOutput capture;
        CommandHandler::HandleEngineOn(car);
        REQUIRE(capture.getOutput().find("Failed to turn on engine") != std::string::npos);
    }

    SECTION("Ошибка выключения при движении")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        car.SetSpeed(10);

        CaptureOutput capture;
        CommandHandler::HandleEngineOff(car);
        REQUIRE(capture.getOutput().find("Failed to turn off engine") != std::string::npos);
    }
}

TEST_CASE("Обработка передач через CommandHandler")
{
    SECTION("Успешная установка передачи")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetGear(car, "1");
        REQUIRE(car.GetGear() == 1);
        REQUIRE(capture.getOutput().empty());
    }

    SECTION("Ошибка: пустой аргумент")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetGear(car, "");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);
    }

    SECTION("Ошибка: нечисловой аргумент")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetGear(car, "abc");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);
    }

    SECTION("Ошибка: недопустимая передача")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetGear(car, "6");
        REQUIRE(capture.getOutput().find("Failed to set gear") != std::string::npos);
    }

    SECTION("Ошибка: задний ход при движении")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        car.SetSpeed(10);

        CaptureOutput capture;
        CommandHandler::HandleSetGear(car, "-1");
        REQUIRE(capture.getOutput().find("Failed to set gear") != std::string::npos);
    }
}

TEST_CASE("Обработка скорости через CommandHandler")
{
    SECTION("Успешная установка скорости")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);

        CaptureOutput capture;
        CommandHandler::HandleSetSpeed(car, "20");
        REQUIRE(car.GetSpeed() == 20);
        REQUIRE(capture.getOutput().empty());
    }

    SECTION("Ошибка: пустой аргумент")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetSpeed(car, "");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);
    }

    SECTION("Ошибка: нечисловой аргумент")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetSpeed(car, "abc");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);
    }

    SECTION("Ошибка: отрицательная скорость")
    {
        Car car;
        car.TurnOnEngine();

        CaptureOutput capture;
        CommandHandler::HandleSetSpeed(car, "-1");
        REQUIRE(capture.getOutput().find("Failed to set speed") != std::string::npos);
    }

    SECTION("Ошибка: скорость вне диапазона передачи")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);

        CaptureOutput capture;
        CommandHandler::HandleSetSpeed(car, "31");
        REQUIRE(capture.getOutput().find("Failed to set speed") != std::string::npos);
    }
}

TEST_CASE("Вывод информации через CommandHandler")
{
    SECTION("Вывод состояния автомобиля")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        car.SetSpeed(25);

        CaptureOutput capture;
        CommandHandler::PrintInfo(car);

        std::string output = capture.getOutput();
        REQUIRE(output.find("Engine: on") != std::string::npos);
        REQUIRE(output.find("Direction: forward") != std::string::npos);
        REQUIRE(output.find("Speed: 25") != std::string::npos);
        REQUIRE(output.find("Gear: 1") != std::string::npos);
    }

    SECTION("Вывод состояния после остановки")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(-1);
        car.SetSpeed(10);
        car.SetSpeed(0);
        car.SetGear(0);
        car.TurnOffEngine();

        CaptureOutput capture;
        CommandHandler::PrintInfo(car);

        std::string output = capture.getOutput();
        REQUIRE(output.find("Engine: off") != std::string::npos);
        REQUIRE(output.find("Direction: standing still") != std::string::npos);
        REQUIRE(output.find("Speed: 0") != std::string::npos);
        REQUIRE(output.find("Gear: 0") != std::string::npos);
    }
}

TEST_CASE("Парсинг команд через CommandHandler")
{
    SECTION("Команда Info")
    {
        auto cmd = CommandHandler::ParseCommand("Info");
        REQUIRE(cmd.type == CommandType::INFO);
        REQUIRE_FALSE(cmd.hasArgument);
    }

    SECTION("Команда EngineOn")
    {
        auto cmd = CommandHandler::ParseCommand("EngineOn");
        REQUIRE(cmd.type == CommandType::ENGINE_ON);
        REQUIRE_FALSE(cmd.hasArgument);
    }

    SECTION("Команда EngineOff")
    {
        auto cmd = CommandHandler::ParseCommand("EngineOff");
        REQUIRE(cmd.type == CommandType::ENGINE_OFF);
        REQUIRE_FALSE(cmd.hasArgument);
    }

    SECTION("Команда SetGear с аргументом")
    {
        auto cmd = CommandHandler::ParseCommand("SetGear 3");
        REQUIRE(cmd.type == CommandType::SET_GEAR);
        REQUIRE(cmd.hasArgument);
        REQUIRE(cmd.argument == "3");
    }

    SECTION("Команда SetGear без аргумента")
    {
        auto cmd = CommandHandler::ParseCommand("SetGear");
        REQUIRE(cmd.type == CommandType::SET_GEAR);
        REQUIRE_FALSE(cmd.hasArgument);
    }

    SECTION("Команда SetSpeed с аргументом")
    {
        auto cmd = CommandHandler::ParseCommand("SetSpeed 50");
        REQUIRE(cmd.type == CommandType::SET_SPEED);
        REQUIRE(cmd.hasArgument);
        REQUIRE(cmd.argument == "50");
    }

    SECTION("Команда SetSpeed с отрицательным аргументом")
    {
        auto cmd = CommandHandler::ParseCommand("SetSpeed -10");
        REQUIRE(cmd.type == CommandType::SET_SPEED);
        REQUIRE(cmd.hasArgument);
        REQUIRE(cmd.argument == "-10");
    }

    SECTION("Неизвестная команда")
    {
        auto cmd = CommandHandler::ParseCommand("unknown");
        REQUIRE(cmd.type == CommandType::UNKNOWN);
        REQUIRE_FALSE(cmd.hasArgument);
    }

    SECTION("Регистронезависимость команд")
    {
        auto cmd = CommandHandler::ParseCommand("info");
        REQUIRE(cmd.type == CommandType::INFO);

        cmd = CommandHandler::ParseCommand("ENGINEOFF");
        REQUIRE(cmd.type == CommandType::ENGINE_OFF);
    }
}

TEST_CASE("Обработка команд через ProcessCommand")
{
    SECTION("Обработка команды Info")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        car.SetSpeed(25);

        CaptureOutput capture;
        CommandHandler::ProcessCommand(car, "Info");

        std::string output = capture.getOutput();
        REQUIRE(output.find("Engine: on") != std::string::npos);
        REQUIRE(output.find("Direction: forward") != std::string::npos);
        REQUIRE(output.find("Speed: 25") != std::string::npos);
        REQUIRE(output.find("Gear: 1") != std::string::npos);
    }

    SECTION("Обработка команды EngineOn")
    {
        Car car;
        CommandHandler::ProcessCommand(car, "EngineOn");
        REQUIRE(car.GetEngine());
    }

    SECTION("Обработка команды EngineOff")
    {
        Car car;
        car.TurnOnEngine();
        CommandHandler::ProcessCommand(car, "EngineOff");
        REQUIRE_FALSE(car.GetEngine());
    }

    SECTION("Обработка команды SetGear с валидным аргументом")
    {
        Car car;
        car.TurnOnEngine();
        CommandHandler::ProcessCommand(car, "SetGear 1");
        REQUIRE(car.GetGear() == 1);
    }

    SECTION("Обработка команды SetGear без аргумента")
    {
        Car car;
        CaptureOutput capture;
        CommandHandler::ProcessCommand(car, "SetGear");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);
    }

    SECTION("Обработка команды SetSpeed с валидным аргументом")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(1);
        CommandHandler::ProcessCommand(car, "SetSpeed 20");
        REQUIRE(car.GetSpeed() == 20);
    }

    SECTION("Обработка неизвестной команды")
    {
        Car car;
        CaptureOutput capture;
        CommandHandler::ProcessCommand(car, "unknown");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
    }
}

TEST_CASE("Интеграционные сценарии через ProcessCommand")
{
    SECTION("Полный цикл движения вперед")
    {
        Car car;

        CommandHandler::ProcessCommand(car, "EngineOn");
        REQUIRE(car.GetEngine());

        CommandHandler::ProcessCommand(car, "SetGear 1");
        REQUIRE(car.GetGear() == 1);

        CommandHandler::ProcessCommand(car, "SetSpeed 20");
        REQUIRE(car.GetSpeed() == 20);
        REQUIRE(car.GetDirection() == "forward");

        CommandHandler::ProcessCommand(car, "SetSpeed 0");
        CommandHandler::ProcessCommand(car, "SetGear 0");
        CommandHandler::ProcessCommand(car, "EngineOff");

        REQUIRE_FALSE(car.GetEngine());
        REQUIRE(car.GetSpeed() == 0);
        REQUIRE(car.GetGear() == 0);
    }

    SECTION("Полный цикл движения назад")
    {
        Car car;

        CommandHandler::ProcessCommand(car, "EngineOn");
        CommandHandler::ProcessCommand(car, "SetGear -1");
        CommandHandler::ProcessCommand(car, "SetSpeed 15");

        REQUIRE(car.GetDirection() == "backward");
        REQUIRE(car.GetSpeed() == 15);

        CommandHandler::ProcessCommand(car, "SetSpeed 0");
        CommandHandler::ProcessCommand(car, "SetGear 0");
        CommandHandler::ProcessCommand(car, "EngineOff");

        REQUIRE_FALSE(car.GetEngine());
    }

    SECTION("Попытка включения заднего хода при движении вперед")
    {
        Car car;

        CommandHandler::ProcessCommand(car, "EngineOn");
        CommandHandler::ProcessCommand(car, "SetGear 1");
        CommandHandler::ProcessCommand(car, "SetSpeed 10");

        CaptureOutput capture;
        CommandHandler::ProcessCommand(car, "SetGear -1");

        REQUIRE(capture.getOutput().find("Failed to set gear") != std::string::npos);
        REQUIRE(car.GetGear() == 1);
    }

    SECTION("Попытка выключения двигателя на скорости")
    {
        Car car;

        CommandHandler::ProcessCommand(car, "EngineOn");
        CommandHandler::ProcessCommand(car, "SetGear 1");
        CommandHandler::ProcessCommand(car, "SetSpeed 30");

        CaptureOutput capture;
        CommandHandler::ProcessCommand(car, "EngineOff");

        REQUIRE(capture.getOutput().find("Failed to turn off engine") != std::string::npos);
        REQUIRE(car.GetEngine());
    }

    SECTION("Множественные ошибки и восстановление")
    {
        Car car;
        CaptureOutput capture;

        CommandHandler::ProcessCommand(car, "SetGear 1");
        REQUIRE(capture.getOutput().find("Failed to set gear") != std::string::npos);

        CommandHandler::ProcessCommand(car, "SetSpeed 50");
        REQUIRE(capture.getOutput().find("Failed to set speed") != std::string::npos);

        CommandHandler::ProcessCommand(car, "EngineOn");
        CommandHandler::ProcessCommand(car, "SetGear 1");
        CommandHandler::ProcessCommand(car, "SetSpeed 25");

        REQUIRE(car.GetEngine());
        REQUIRE(car.GetGear() == 1);
        REQUIRE(car.GetSpeed() == 25);
    }
}
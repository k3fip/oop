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
    CaptureOutput() : old(std::cout.rdbuf(buffer.rdbuf())) { }
    ~CaptureOutput() { std::cout.rdbuf(old); }
    std::string getOutput() { return buffer.str(); }
    void clear() { buffer.str(""); buffer.clear(); }

private:
    std::stringstream buffer;
    std::streambuf* old;
};

TEST_CASE("Начальное состояние")
{
    Car car;
    REQUIRE_FALSE(car.GetEngine());
    REQUIRE(car.GetSpeed() == 0);
    REQUIRE(car.GetGear() == 0);
    REQUIRE(car.GetDirection() == "standing still");
}

TEST_CASE("Двигатель")
{
    Car car;

    SECTION("Включение/выключение")
    {
        REQUIRE(car.TurnOnEngine());
        REQUIRE(car.GetEngine());
        REQUIRE(car.TurnOffEngine());
        REQUIRE_FALSE(car.GetEngine());
    }

    SECTION("Нельзя включить при включенной передаче")
    {
        car.TurnOnEngine();
        car.SetGear(1);
        car.TurnOffEngine();
        REQUIRE_FALSE(car.TurnOnEngine());
    }

    SECTION("Нельзя выключить при движении")
    {
        car.TurnOnEngine();
        car.SetGear(1);
        car.SetSpeed(10);
        REQUIRE_FALSE(car.TurnOffEngine());
    }
}

TEST_CASE("Передачи")
{
    Car car;
    car.TurnOnEngine();

    SECTION("Задний ход - только на скорости 0")
    {
        REQUIRE(car.SetGear(-1));
        car.SetGear(1);
        car.SetSpeed(10);
        REQUIRE_FALSE(car.SetGear(-1));
        car.SetSpeed(0);
        REQUIRE(car.SetGear(-1));
    }

    SECTION("С заднего на переднюю - только на 0")
    {
        car.SetGear(-1);
        car.SetSpeed(10);
        REQUIRE_FALSE(car.SetGear(1));
        car.SetSpeed(0);
        REQUIRE(car.SetGear(1));
    }
}

TEST_CASE("Последовательный разгон и торможение")
{
    Car car;
    car.TurnOnEngine();

    SECTION("Разгон")
    {
        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(0));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.GetGear() == 1);
        REQUIRE(car.GetDirection() == "forward");

        REQUIRE(car.SetGear(2));
        REQUIRE(car.SetSpeed(20));
        REQUIRE(car.SetSpeed(50));
        REQUIRE(car.GetGear() == 2);

        REQUIRE(car.SetGear(3));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.SetSpeed(60));
        REQUIRE(car.GetGear() == 3);

        REQUIRE(car.SetGear(4));
        REQUIRE(car.SetSpeed(40));
        REQUIRE(car.SetSpeed(90));
        REQUIRE(car.GetGear() == 4);

        REQUIRE(car.SetGear(5));
        REQUIRE(car.SetSpeed(50));
        REQUIRE(car.SetSpeed(150));
        REQUIRE(car.GetGear() == 5);
    }

    SECTION("Торможение")
    {
        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.SetGear(2));
        REQUIRE(car.SetSpeed(50));
        REQUIRE(car.SetGear(3));
        REQUIRE(car.SetSpeed(60));
        REQUIRE(car.SetGear(4));
        REQUIRE(car.SetSpeed(90));
        REQUIRE(car.SetGear(5));
        REQUIRE(car.SetSpeed(150));

        REQUIRE(car.SetSpeed(50));
        REQUIRE(car.SetGear(4));
        REQUIRE(car.SetSpeed(40));
        REQUIRE(car.SetGear(3));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.SetGear(2));
        REQUIRE(car.SetSpeed(20));
        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(0));
        REQUIRE(car.SetGear(0));
    }

    SECTION("Задний ход")
    {
        REQUIRE(car.SetGear(-1));
        REQUIRE(car.SetSpeed(0));
        REQUIRE(car.SetSpeed(20));
        REQUIRE(car.GetDirection() == "backward");
        REQUIRE_FALSE(car.SetSpeed(21));
    }

    SECTION("Нейтраль")
    {
        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.SetGear(0));
        REQUIRE_FALSE(car.SetSpeed(31));
        REQUIRE(car.SetSpeed(20));
        REQUIRE_FALSE(car.SetSpeed(30));
    }
}

TEST_CASE("Направление")
{
    Car car;
    car.TurnOnEngine();

    CHECK(car.GetDirection() == "standing still");

    car.SetGear(1);
    car.SetSpeed(10);
    CHECK(car.GetDirection() == "forward");

    car.SetGear(0);
    car.SetSpeed(0);

    car.SetGear(-1);
    car.SetSpeed(5);
    CHECK(car.GetDirection() == "backward");

    car.SetGear(0);
    car.SetSpeed(0);

    CHECK(car.GetDirection() == "standing still");
}

TEST_CASE("Сценарии использования")
{
    SECTION("Полный цикл вперед-назад")
    {
        Car car;
        REQUIRE(car.TurnOnEngine());

        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(20));
        REQUIRE(car.GetDirection() == "forward");

        REQUIRE(car.SetGear(2));
        REQUIRE(car.SetSpeed(30));
        REQUIRE(car.SetSpeed(20));
        REQUIRE(car.SetGear(1));
        REQUIRE(car.SetSpeed(0));
        REQUIRE(car.SetGear(0));

        REQUIRE(car.SetGear(-1));
        REQUIRE(car.SetSpeed(15));
        REQUIRE(car.GetDirection() == "backward");

        REQUIRE(car.SetSpeed(0));
        REQUIRE(car.SetGear(0));
        REQUIRE(car.TurnOffEngine());
    }

    SECTION("Запрещенные действия")
    {
        Car car;
        car.TurnOnEngine();

        REQUIRE_FALSE(car.SetGear(2));

        car.SetGear(1);
        car.SetSpeed(5);
        REQUIRE_FALSE(car.SetGear(-1));

        car.SetGear(0);
        REQUIRE_FALSE(car.SetSpeed(10));
    }
}

TEST_CASE("Вывод сообщений об ошибках")
{
    SECTION("Ошибки двигателя")
    {
        Car car;
        CaptureOutput capture;

        car.TurnOnEngine();
        car.SetGear(1);
        car.TurnOffEngine();
        capture.clear();
        car.TurnOnEngine();
        REQUIRE(capture.getOutput().find("Car must be stopped and in neutral gear") != std::string::npos);

        Car car2;
        car2.TurnOnEngine();
        car2.SetGear(1);
        car2.SetSpeed(10);
        capture.clear();
        car2.TurnOffEngine();
        REQUIRE(capture.getOutput().find("Car must be stopped and in neutral gear") != std::string::npos);
    }

    SECTION("Ошибки установки передачи")
    {
        Car car;
        CaptureOutput capture;

        capture.clear();
        car.SetGear(1);
        REQUIRE(capture.getOutput().find("Cannot set gear while engine is off") != std::string::npos);

        car.TurnOnEngine();

        // Неверная передача
        capture.clear();
        car.SetGear(6);
        REQUIRE(capture.getOutput().find("Invalid gear") != std::string::npos);

        capture.clear();
        car.SetGear(-2);
        REQUIRE(capture.getOutput().find("Invalid gear") != std::string::npos);

        car.SetGear(1);
        car.SetSpeed(10);
        capture.clear();
        car.SetGear(-1);
        REQUIRE(capture.getOutput().find("Cannot reverse while moving") != std::string::npos);

        Car car3;
        car3.TurnOnEngine();
        car3.SetGear(1);
        car3.SetSpeed(30);
        car3.SetGear(2);
        car3.SetSpeed(50);
        capture.clear();
        car3.SetGear(1); 
        REQUIRE(capture.getOutput().find("Unsuitable current speed") != std::string::npos);
    }

    SECTION("Ошибки установки скорости")
    {
        Car car;
        CaptureOutput capture;

        capture.clear();
        car.SetSpeed(10);
        REQUIRE(capture.getOutput().find("Cannot set speed while engine is off") != std::string::npos);

        car.TurnOnEngine();

        capture.clear();
        car.SetSpeed(-1);
        REQUIRE(capture.getOutput().find("Speed must be in [ 0; 150 ]") != std::string::npos);

        capture.clear();
        car.SetSpeed(151);
        REQUIRE(capture.getOutput().find("Speed must be in [ 0; 150 ]") != std::string::npos);

        car.SetGear(1);
        car.SetSpeed(20);
        car.SetGear(0);
        capture.clear();
        car.SetSpeed(25);
        REQUIRE(capture.getOutput().find("Cannot accelerate on neutral") != std::string::npos);

        Car car4;
        car4.TurnOnEngine();
        car4.SetGear(1);
        capture.clear();
        car4.SetSpeed(31);
        REQUIRE(capture.getOutput().find("Speed is out of gear range") != std::string::npos);
    }

    SECTION("Ошибки при обработке команд")
    {
        Car car;
        CaptureOutput capture;

        capture.clear();
        ProcessCommand(car, "SetGear");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);

        capture.clear();
        ProcessCommand(car, "SetSpeed");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);

        capture.clear();
        ProcessCommand(car, "SetGear abc");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);

        capture.clear();
        ProcessCommand(car, "unknown");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
    }
}

TEST_CASE("Обработка команд")
{
    Car car;
    car.TurnOnEngine();

    SECTION("Info")
    {
        car.SetGear(1);
        car.SetSpeed(25);

        CaptureOutput capture;
        ProcessCommand(car, "Info");

        std::string output = capture.getOutput();
        REQUIRE(output.find("Engine: on") != std::string::npos);
        REQUIRE(output.find("Direction: forward") != std::string::npos);
        REQUIRE(output.find("Speed: 25") != std::string::npos);
        REQUIRE(output.find("Gear: 1") != std::string::npos);
    }

    SECTION("Ошибочные команды")
    {
        CaptureOutput capture;

        ProcessCommand(car, "SetGear abc");
        REQUIRE(capture.getOutput().find("Invalid command argument") != std::string::npos);

        ProcessCommand(car, "SetSpeed -10");
        REQUIRE(capture.getOutput().find("Speed must be in [ 0; 150 ]") != std::string::npos);

        ProcessCommand(car, "unknown");
        REQUIRE(capture.getOutput().find("Unknown command") != std::string::npos);
    }

    SECTION("Команды при выключенном двигателе")
    {
        Car carOff;
        CaptureOutput capture;

        ProcessCommand(carOff, "SetGear 1");
        REQUIRE(capture.getOutput().find("Cannot set gear while engine is off") != std::string::npos);
    }
}

TEST_CASE("Вспомогательные функции")
{
    SECTION("IsInteger")
    {
        REQUIRE(IsInteger("0"));
        REQUIRE(IsInteger("123"));
        REQUIRE(IsInteger("-5"));
        REQUIRE_FALSE(IsInteger(""));
        REQUIRE_FALSE(IsInteger("12.3"));
    }

    SECTION("ParseCommand")
    {
        REQUIRE(ParseCommand("Info").type == CommandType::INFO);
        REQUIRE(ParseCommand("SetGear 3").type == CommandType::SET_GEAR);
        REQUIRE(ParseCommand("unknown").type == CommandType::UNKNOWN);
    }
}
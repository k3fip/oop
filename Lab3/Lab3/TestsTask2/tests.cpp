#include <catch2/catch_all.hpp>
#include "../Task2/Parser.h"
#include "../Task2/Commands.h"
#include "../Task2/ExecutionContext.h"
#include "../Task2/Expressions.h"
#include "../Task2/Interpreter.h"
#include <cmath>

TEST_CASE("Разбор команды var") {
    SECTION("Корректное объявление переменной") {
        auto result = Parser::Parse("var x");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Объявление без имени") {
        auto result = Parser::Parse("var");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Слишком много аргументов") {
        auto result = Parser::Parse("var x y");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Некорректный идентификатор (начинается с цифры)") {
        auto result = Parser::Parse("var 123test");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Некорректный идентификатор (спецсимволы)") {
        auto result = Parser::Parse("var test@var");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Пробелы в команде") {
        auto result = Parser::Parse("var   myVar");
        REQUIRE(result.status == Status::Ok);
    }
}

TEST_CASE("Разбор команды let") {
    SECTION("Присваивание числа") {
        auto result = Parser::Parse("let x = 42");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Присваивание дробного числа") {
        auto result = Parser::Parse("let x = 3.14159");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Присваивание значения переменной") {
        auto result = Parser::Parse("let x = y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Без пробелов вокруг равно") {
        auto result = Parser::Parse("let x=42");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Без значения") {
        auto result = Parser::Parse("let x =");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Без знака равно") {
        auto result = Parser::Parse("let x 42");
        REQUIRE(result.errorMessage == "Invalid usage");
    }
}

TEST_CASE("Разбор команды fn") {
    SECTION("Простое присваивание") {
        auto result = Parser::Parse("fn f = x");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Сложение") {
        auto result = Parser::Parse("fn sum = x + y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Вычитание") {
        auto result = Parser::Parse("fn diff = x - y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Умножение") {
        auto result = Parser::Parse("fn prod = x * y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Деление") {
        auto result = Parser::Parse("fn quot = x / y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Без пробелов вокруг оператора") {
        auto result = Parser::Parse("fn sum=x+y");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Некорректный оператор") {
        auto result = Parser::Parse("fn sum = x % y");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("Без выражения") {
        auto result = Parser::Parse("fn sum =");
        REQUIRE(result.errorMessage == "Invalid usage");
    }
}

TEST_CASE("Разбор команды print") {
    SECTION("Печать переменной") {
        auto result = Parser::Parse("print x");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("Без имени") {
        auto result = Parser::Parse("print");
        REQUIRE(result.errorMessage == "Invalid usage");
    }
}

TEST_CASE("Разбор команд printvars и printfns") {
    SECTION("printvars без аргументов") {
        auto result = Parser::Parse("printvars");
        REQUIRE(result.status == Status::Ok);
    }

    SECTION("printvars с лишними аргументами") {
        auto result = Parser::Parse("printvars x");
        REQUIRE(result.errorMessage == "Invalid usage");
    }

    SECTION("printfns без аргументов") {
        auto result = Parser::Parse("printfns");
        REQUIRE(result.status == Status::Ok);
    }
}

TEST_CASE("Обработка некорректного ввода") {
    SECTION("Неизвестная команда") {
        auto result = Parser::Parse("unknown x");
        REQUIRE(result.errorMessage == "Unknown command");
    }

    SECTION("Пустая строка") {
        auto result = Parser::Parse("");
        REQUIRE(result.errorMessage == "Unknown command");
    }

    SECTION("Только пробелы") {
        auto result = Parser::Parse("   ");
        REQUIRE(result.errorMessage == "Unknown command");
    }

    SECTION("Регистр букв") {
        auto result = Parser::Parse("VAR x");
        REQUIRE(result.errorMessage == "Unknown command");
    }
}

TEST_CASE("Работа с переменными") {
    ExecutionContext ctx;

    SECTION("Объявление переменной") {
        ctx.DeclareVariable("x");
        REQUIRE(ctx.Contains("x"));
        REQUIRE(std::isnan(ctx.GetValue("x")));
    }

    SECTION("Повторное объявление вызывает ошибку") {
        ctx.DeclareVariable("x");
        auto err = ctx.DeclareVariable("x");
        REQUIRE(err == ErrorCode::NameAlreadyExists);
    }

    SECTION("Установка значения") {
        ctx.DeclareVariable("x");
        ctx.SetVariable("x", 42.0);
        REQUIRE(ctx.GetValue("x") == 42.0);
    }

    SECTION("Автосоздание переменной при присваивании") {
        ctx.SetVariable("x", 42.0);
        REQUIRE(ctx.GetValue("x") == 42.0);
    }

    SECTION("Нельзя присвоить значение функции") {
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));
        auto err = ctx.SetVariable("f", 42.0);
        REQUIRE(err == ErrorCode::IsFunctionName);
    }

    SECTION("Получение несуществующей переменной возвращает NaN") {
        REQUIRE(std::isnan(ctx.GetValue("nonexistent")));
    }
}

TEST_CASE("Работа с функциями") {
    ExecutionContext ctx;

    SECTION("Добавление функции") {
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));
        REQUIRE(ctx.Contains("f"));
    }

    SECTION("Повторное имя функции вызывает ошибку") {
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));
        auto err = ctx.AddFunction("f", std::make_unique<VariableExpression>("y"));
        REQUIRE(err == ErrorCode::NameAlreadyExists);
    }

    SECTION("Функция зависит от значения переменной") {
        ctx.SetVariable("x", 5.0);
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));

        REQUIRE(ctx.GetValue("f") == 5.0);

        ctx.SetVariable("x", 10.0);
        REQUIRE(ctx.GetValue("f") == 10.0);
    }

    SECTION("Несуществующая функция возвращает NaN") {
        REQUIRE(std::isnan(ctx.GetValue("nonexistent")));
    }
}

TEST_CASE("Получение списков переменных и функций") {
    SECTION("Переменные сортируются по алфавиту") {
        ExecutionContext ctx;
        ctx.SetVariable("z", 3.0);
        ctx.SetVariable("a", 1.0);
        ctx.SetVariable("m", 2.0);

        auto vars = ctx.GetVariables();
        REQUIRE(vars.size() == 3);
        REQUIRE(vars[0].name == "a");
        REQUIRE(vars[1].name == "m");
        REQUIRE(vars[2].name == "z");
    }

    SECTION("Пустой список переменных") {
        ExecutionContext ctx;
        REQUIRE(ctx.GetVariables().empty());
    }

    SECTION("Функции сортируются по алфавиту") {
        ExecutionContext ctx;
        ctx.SetVariable("x", 10.0);
        ctx.AddFunction("z_func", std::make_unique<VariableExpression>("x"));
        ctx.AddFunction("a_func", std::make_unique<VariableExpression>("x"));

        auto funcs = ctx.GetFunctions();
        REQUIRE(funcs.size() == 2);
        REQUIRE(funcs[0].name == "a_func");
        REQUIRE(funcs[1].name == "z_func");
    }
}

TEST_CASE("Числовое выражение") {
    ExecutionContext ctx;

    SECTION("Целое число") {
        NumberExpression expr(42);
        REQUIRE(expr.Evaluate(ctx) == 42.0);
    }

    SECTION("Дробное число") {
        NumberExpression expr(3.14);
        REQUIRE(expr.Evaluate(ctx) == 3.14);
    }

    SECTION("Ноль") {
        NumberExpression expr(0.0);
        REQUIRE(expr.Evaluate(ctx) == 0.0);
    }

    SECTION("Отрицательное число") {
        NumberExpression expr(-42.0);
        REQUIRE(expr.Evaluate(ctx) == -42.0);
    }
}

TEST_CASE("Переменное выражение") {
    SECTION("Существующая переменная") {
        ExecutionContext ctx;
        ctx.SetVariable("x", 42.0);

        VariableExpression expr("x");
        REQUIRE(expr.Evaluate(ctx) == 42.0);
    }

    SECTION("Несуществующая переменная") {
        ExecutionContext ctx;
        VariableExpression expr("x");
        REQUIRE(std::isnan(expr.Evaluate(ctx)));
    }

    SECTION("Значение обновляется при изменении переменной") {
        ExecutionContext ctx;
        ctx.SetVariable("x", 10.0);
        VariableExpression expr("x");

        REQUIRE(expr.Evaluate(ctx) == 10.0);
        ctx.SetVariable("x", 20.0);
        REQUIRE(expr.Evaluate(ctx) == 20.0);
    }

    SECTION("Ссылка на функцию") {
        ExecutionContext ctx;
        ctx.SetVariable("x", 5.0);
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));

        VariableExpression expr("f");
        REQUIRE(expr.Evaluate(ctx) == 5.0);
    }
}

TEST_CASE("Бинарное выражение") {
    ExecutionContext ctx;
    ctx.SetVariable("x", 10.0);
    ctx.SetVariable("y", 5.0);

    SECTION("Сложение") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '+'
        );
        REQUIRE(expr.Evaluate(ctx) == 15.0);
    }

    SECTION("Вычитание") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '-'
        );
        REQUIRE(expr.Evaluate(ctx) == 5.0);
    }

    SECTION("Умножение") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '*'
        );
        REQUIRE(expr.Evaluate(ctx) == 50.0);
    }

    SECTION("Деление") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '/'
        );
        REQUIRE(expr.Evaluate(ctx) == 2.0);
    }

    SECTION("Деление на ноль возвращает NaN") {
        ctx.SetVariable("y", 0.0);
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '/'
        );
        REQUIRE(std::isnan(expr.Evaluate(ctx)));
    }

    SECTION("NaN в левом операнде") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("undefined"),
            std::make_unique<VariableExpression>("y"),
            '+'
        );
        REQUIRE(std::isnan(expr.Evaluate(ctx)));
    }

    SECTION("NaN в правом операнде") {
        BinaryExpression expr(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("undefined"),
            '+'
        );
        REQUIRE(std::isnan(expr.Evaluate(ctx)));
    }

    SECTION("Вложенные выражения") {
        BinaryExpression expr(
            std::make_unique<BinaryExpression>(
                std::make_unique<VariableExpression>("x"),
                std::make_unique<VariableExpression>("y"),
                '+'
            ),
            std::make_unique<NumberExpression>(2),
            '*'
        );
        REQUIRE(expr.Evaluate(ctx) == 30.0);
    }

    SECTION("Некорректный оператор вызывает исключение") {
        REQUIRE_THROWS_AS(
            BinaryExpression(
                std::make_unique<NumberExpression>(1),
                std::make_unique<NumberExpression>(2),
                '%'
            ),
            std::invalid_argument
        );
    }
}

std::vector<std::string> ExecuteCommand(ICommand& cmd, ExecutionContext& ctx) {
    std::vector<std::string> output;
    cmd.Execute(ctx, [&](const std::string& msg) { output.push_back(msg); });
    return output;
}

TEST_CASE("Команда var") {
    ExecutionContext ctx;

    SECTION("Объявление новой переменной") {
        VarCommand cmd("x");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.empty());
        REQUIRE(ctx.Contains("x"));
        REQUIRE(std::isnan(ctx.GetValue("x")));
    }

    SECTION("Повторное объявление") {
        ctx.DeclareVariable("x");
        VarCommand cmd("x");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name already exists");
    }
}

TEST_CASE("Команда let") {
    ExecutionContext ctx;

    SECTION("Присваивание числа новой переменной") {
        LetCommand cmd("x", 42.0);
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.empty());
        REQUIRE(ctx.GetValue("x") == 42.0);
    }

    SECTION("Изменение существующей переменной") {
        ctx.DeclareVariable("x");
        LetCommand cmd("x", 100.0);
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.empty());
        REQUIRE(ctx.GetValue("x") == 100.0);
    }

    SECTION("Присваивание из другой переменной") {
        ctx.SetVariable("y", 50.0);
        LetCommand cmd("x", std::string("y"));
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.empty());
        REQUIRE(ctx.GetValue("x") == 50.0);
    }

    SECTION("Присваивание из несуществующей переменной") {
        LetCommand cmd("x", std::string("nonexistent"));
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name does not exist");
    }

    SECTION("Присваивание имени функции вызывает ошибку") {
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));
        LetCommand cmd("f", 42.0);
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Invalid usage");
    }

    SECTION("Let сохраняет снимок значения") {
        ctx.SetVariable("y", 10.0);

        LetCommand cmd("x", std::string("y"));
        ExecuteCommand(cmd, ctx);

        ctx.SetVariable("y", 20.0);
        REQUIRE(ctx.GetValue("x") == 10.0);
    }
}

TEST_CASE("Команда fn") {
    ExecutionContext ctx;

    SECTION("Создание функции") {
        ctx.SetVariable("x", 5.0);
        FnCommand cmd("f", std::make_unique<VariableExpression>("x"));
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.empty());
        REQUIRE(ctx.Contains("f"));
        REQUIRE(ctx.GetValue("f") == 5.0);
    }

    SECTION("Повторное имя функции") {
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));
        FnCommand cmd("f", std::make_unique<VariableExpression>("y"));
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name already exists");
    }

    SECTION("Fn создает динамическую ссылку") {
        ctx.SetVariable("x", 10.0);

        FnCommand cmd("f", std::make_unique<VariableExpression>("x"));
        ExecuteCommand(cmd, ctx);

        REQUIRE(ctx.GetValue("f") == 10.0);

        ctx.SetVariable("x", 20.0);
        REQUIRE(ctx.GetValue("f") == 20.0);
    }

    SECTION("Fn с бинарным выражением") {
        ctx.SetVariable("x", 10.0);
        ctx.SetVariable("y", 5.0);

        auto body = std::make_unique<BinaryExpression>(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y"),
            '+'
        );

        FnCommand cmd("sum", std::move(body));
        ExecuteCommand(cmd, ctx);

        REQUIRE(ctx.GetValue("sum") == 15.0);
    }
}

TEST_CASE("Команда print") {
    ExecutionContext ctx;

    SECTION("Печать значения переменной") {
        ctx.SetVariable("x", 42.0);
        PrintCommand cmd("x");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "42.00");
    }

    SECTION("Печать NaN") {
        ctx.DeclareVariable("x");
        PrintCommand cmd("x");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "nan");
    }

    SECTION("Печать несуществующего имени") {
        PrintCommand cmd("nonexistent");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name does not exist");
    }

    SECTION("Печать функции") {
        ctx.SetVariable("x", 10.0);
        ctx.AddFunction("f", std::make_unique<VariableExpression>("x"));

        PrintCommand cmd("f");
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "10.00");
    }
}

TEST_CASE("Команда printvars") {
    ExecutionContext ctx;

    SECTION("Печать всех переменных в алфавитном порядке") {
        ctx.SetVariable("z", 3.0);
        ctx.SetVariable("a", 1.0);
        ctx.SetVariable("m", 2.0);

        PrintVarsCommand cmd;
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 3);
        REQUIRE(output[0] == "a:1.00");
        REQUIRE(output[1] == "m:2.00");
        REQUIRE(output[2] == "z:3.00");
    }

    SECTION("Печать пустого списка") {
        PrintVarsCommand cmd;
        auto output = ExecuteCommand(cmd, ctx);
        REQUIRE(output.empty());
    }
}

TEST_CASE("Команда printfns") {
    ExecutionContext ctx;

    SECTION("Печать всех функций в алфавитном порядке") {
        ctx.SetVariable("x", 10.0);
        ctx.AddFunction("z_func", std::make_unique<VariableExpression>("x"));
        ctx.AddFunction("a_func", std::make_unique<NumberExpression>(42.0));

        PrintFnsCommand cmd;
        auto output = ExecuteCommand(cmd, ctx);

        REQUIRE(output.size() == 2);
        REQUIRE(output[0] == "a_func:42.00");
        REQUIRE(output[1] == "z_func:10.00");
    }

    SECTION("Печать пустого списка функций") {
        PrintFnsCommand cmd;
        auto output = ExecuteCommand(cmd, ctx);
        REQUIRE(output.empty());
    }
}

std::vector<std::string> RunInterpreter(const std::string& input) {
    std::stringstream in(input);
    std::vector<std::string> outputs;

    OutputCallback callback = [&](const std::string& msg) {
        outputs.push_back(msg);
        };

    Interpreter interpreter(callback);
    interpreter.Run(in);

    return outputs;
}

TEST_CASE("Пример 1: базовое присваивание") {
    std::string input =
        "var x\n"
        "print x\n"
        "let x=42\n"
        "print x\n"
        "let x=1.234\n"
        "print x\n"
        "let y=x\n"
        "let x=99\n"
        "printvars\n";

    auto output = RunInterpreter(input);

    REQUIRE(output.size() == 5);
    REQUIRE(output[0] == "nan");
    REQUIRE(output[1] == "42.00");
    REQUIRE(output[2] == "1.23");
    REQUIRE(output[3] == "x:99.00");
    REQUIRE(output[4] == "y:1.23");
}

TEST_CASE("Пример 2: функции") {
    std::string input =
        "var x\n"
        "var y\n"
        "fn XPlusY=x+y\n"
        "print XPlusY\n"
        "let x=3\n"
        "let y=4\n"
        "print XPlusY\n"
        "let x=10\n"
        "print XPlusY\n"
        "let z=3.5\n"
        "fn XPlusYDivZ=XPlusY/z\n"
        "printfns\n";

    auto output = RunInterpreter(input);

    REQUIRE(output.size() == 5);
    REQUIRE(output[0] == "nan");
    REQUIRE(output[1] == "7.00");
    REQUIRE(output[2] == "14.00");
    REQUIRE(output[3] == "XPlusY:14.00");
    REQUIRE(output[4] == "XPlusYDivZ:4.00");
}

TEST_CASE("Пример 3: разница между let и fn") {
    std::string input =
        "let v=42\n"
        "let variable=v\n"
        "fn function=v\n"
        "let v=43\n"
        "print variable\n"
        "print function\n";

    auto output = RunInterpreter(input);

    REQUIRE(output.size() == 2);
    REQUIRE(output[0] == "42.00");
    REQUIRE(output[1] == "43.00");
}

TEST_CASE("Пример 4: площадь круга") {
    std::string input =
        "var radius\n"
        "let pi=3.14159265\n"
        "fn radiusSquared=radius*radius\n"
        "fn circleArea=pi*radiusSquared\n"
        "let radius=10\n"
        "print circleArea\n"
        "let circle10Area=circleArea\n"
        "let radius=20\n"
        "let circle20Area=circleArea\n"
        "printfns\n"
        "printvars\n";

    auto output = RunInterpreter(input);

    REQUIRE(output.size() >= 7);
    REQUIRE(output[0] == "314.16");

    bool foundArea1 = false, foundArea2 = false;
    for (const auto& line : output) {
        if (line == "circleArea:1256.64") foundArea1 = true;
        if (line == "radiusSquared:400.00") foundArea2 = true;
    }
    REQUIRE(foundArea1);
    REQUIRE(foundArea2);
}

TEST_CASE("Обработка ошибок") {
    SECTION("Неизвестная команда") {
        auto output = RunInterpreter("unknown x\n");
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Unknown command");
    }

    SECTION("Несуществующее имя") {
        auto output = RunInterpreter("print x\n");
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name does not exist");
    }

    SECTION("Повторное объявление переменной") {
        auto output = RunInterpreter("var x\nvar x\n");
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Name already exists");
    }

    SECTION("Некорректный синтаксис") {
        auto output = RunInterpreter("let x =\n");
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "Invalid usage");
    }
}

TEST_CASE("Крайние случаи") {
    SECTION("Деление на ноль") {
        auto output = RunInterpreter(
            "let x=10\n"
            "let y=0\n"
            "fn f=x/y\n"
            "print f\n"
        );
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "nan");
    }

    SECTION("Все переменные не определены") {
        auto output = RunInterpreter(
            "var a\n"
            "var b\n"
            "var c\n"
            "printvars\n"
        );
        REQUIRE(output.size() == 3);
        for (const auto& line : output) {
            REQUIRE(line.find(":nan") != std::string::npos);
        }
    }

    SECTION("Пустой ввод") {
        auto output = RunInterpreter("\n\n\n");
        REQUIRE(output.empty());
    }

    SECTION("Функция от функции от функции") {
        auto output = RunInterpreter(
            "let x=2\n"
            "fn doubleX=x+x\n"
            "fn quadX=doubleX+doubleX\n"
            "print quadX\n"
        );
        REQUIRE(output.size() == 1);
        REQUIRE(output[0] == "8.00");
    }

    SECTION("Изменение переменной после создания функции") {
        auto output = RunInterpreter(
            "var x\n"
            "fn f=x\n"
            "let x=5\n"
            "print f\n"
            "let x=10\n"
            "print f\n"
        );
        REQUIRE(output.size() == 2);
        REQUIRE(output[0] == "5.00");
        REQUIRE(output[1] == "10.00");
    }
}
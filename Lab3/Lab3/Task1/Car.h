#pragma once

#include <string>
#include <unordered_map>
#include <utility>

const int MIN_GEAR = -1;
const int MAX_GEAR = 5;
const int MIN_SPEED = 0;
const int MAX_SPEED = 150;

static constexpr const char* STANDING_STILL_STR = "standing still";
static constexpr const char* FORWARD_STR = "forward";
static constexpr const char* BACKWARD_STR = "backward";

inline static const std::unordered_map<int, std::pair<int, int>> speedLimits
{
    { -1, { 0, 20 } },
    { 0, { 0, 150 } },
    { 1, { 0, 30 } },
    { 2, { 20, 50 } },
    { 3, { 30, 60 } },
    { 4, { 40, 90 } },
    { 5, { 50, 150 } }
};

enum class Direction
{
    STANDING_STILL,
    FORWARD,
    BACKWARD
};

class Car
{
public:
    bool GetEngine() const;
    int GetSpeed() const;
    int GetGear() const;
    std::string GetDirection() const;

    bool TurnOnEngine();
    bool TurnOffEngine();
    bool SetGear(int gear);
    bool SetSpeed(int speed);

private:
    bool m_isTurnedOn = false;
    int m_gear = 0;
    int m_speed = MIN_SPEED;
    Direction m_direction = Direction::STANDING_STILL;

    void setDirection();
};
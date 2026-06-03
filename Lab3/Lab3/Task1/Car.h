#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>

const int MIN_GEAR = -1;
const int MAX_GEAR = 5;
const int MIN_SPEED = 0;
const int MAX_SPEED = 150;

std::string STANDING_STILL_STR = "standing still";
std::string FORWARD_STR = "forward";
std::string BACKWARD_STR = "backward";

enum class Direction
{
    STANDING_STILL,
    FORWARD,
    BACKWARD
};

static std::unordered_map<int, std::pair<int, int>> speedLimits
{
    { -1, { 0, 20 } },
    { 0, { 0, 150 } },
    { 1, { 0, 30 } },
    { 2, { 20, 50 } },
    { 3, { 30, 60 } },
    { 4, { 40, 90 } },
    { 5, { 50, 150 } }
};

class Car
{
public:
    bool GetEngine() const { return m_isTurnedOn; }
    int GetSpeed() const { return m_speed; }
    int GetGear() const { return m_gear; }
    std::string GetDirection() const
    {
        switch (m_direction)
        {
        case Direction::STANDING_STILL: return STANDING_STILL_STR;
        case Direction::FORWARD: return FORWARD_STR;
        case Direction::BACKWARD: return BACKWARD_STR;
        }
    }
    

    bool TurnOnEngine()
    {
        if (m_gear == 0 && m_speed == MIN_SPEED)
        {
            m_isTurnedOn = true;
            return true;
        }
        else { return false; }
    }

    bool TurnOffEngine()
    {
        if (m_gear == 0 && m_speed == MIN_SPEED)
        {
            m_isTurnedOn = false;
            return true;
        }
        else { return false; }
    }

    bool SetGear(int gear)
    {
        if (gear != 0 && m_isTurnedOn == false) { return false; }

        if (gear < MIN_GEAR || gear > MAX_GEAR) { return false; }

        if (gear == MIN_GEAR && m_speed != MIN_SPEED) { return false; }

        if (speedLimits[gear].first > m_speed || speedLimits[gear].second < m_speed || m_direction == Direction::BACKWARD) { return false; }

        m_gear = gear;
        return true;
    }

    bool SetSpeed(int speed)
    {
        if (m_isTurnedOn == false) { return false; }

        if (speed < MIN_SPEED || speed > MAX_SPEED) { return false; }

        if (m_gear == 0 && speed > m_speed) { return false; }

        if (speedLimits[m_gear].first > speed || speedLimits[m_gear].second < speed) { return false; }

        m_speed = speed;
        setDirection();
        return true;
    }

private:

    bool m_isTurnedOn = false;
    int m_gear = 0;
    int m_speed = MIN_SPEED;
    Direction m_direction = Direction::STANDING_STILL;

    

    void setDirection()
    {
        if (m_speed == MIN_SPEED) { m_direction = Direction::STANDING_STILL; }
        else if (m_gear == MIN_GEAR && m_speed != MIN_SPEED) { m_direction = Direction::BACKWARD; }
        else if (m_gear != MIN_GEAR && m_speed != MIN_SPEED) { m_direction = Direction::FORWARD; }
    }
};
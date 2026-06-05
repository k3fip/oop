#include "Car.h"
#include <iostream>

bool Car::GetEngine() const { return m_isTurnedOn; }

int Car::GetSpeed() const { return m_speed; }

int Car::GetGear() const { return m_gear; }

std::string Car::GetDirection() const
{
    switch (m_direction)
    {
    case Direction::STANDING_STILL: return STANDING_STILL_STR;
    case Direction::FORWARD: return FORWARD_STR;
    case Direction::BACKWARD: return BACKWARD_STR;
    }
}

bool Car::TurnOnEngine()
{
    if (m_gear == 0 && m_speed == MIN_SPEED)
    {
        m_isTurnedOn = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool Car::TurnOffEngine()
{
    if (m_gear == 0 && m_speed == MIN_SPEED)
    {
        m_isTurnedOn = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool Car::SetGear(int gear)
{
    if (gear != 0 && m_isTurnedOn == false)
    {
        return false;
    }

    if (gear < MIN_GEAR || gear > MAX_GEAR)
    {
        return false;
    }

    if (gear == MIN_GEAR && m_speed != MIN_SPEED)
    {
        return false;
    }

    const auto& limits = speedLimits.find(gear)->second;
    if (limits.first > m_speed || 
        limits.second < m_speed ||
        m_direction == Direction::BACKWARD)
    {
        return false;
    }

    m_gear = gear;
    return true;
}

bool Car::SetSpeed(int speed)
{
    if (m_isTurnedOn == false)
    {
        return false;
    }

    if (speed < MIN_SPEED || speed > MAX_SPEED)
    {
        return false;
    }

    if (m_gear == 0 && speed > m_speed)
    {
        return false;
    }

    const auto& limits = speedLimits.find(m_gear)->second;
    if (limits.first > speed || limits.second < speed)
    {
        return false;
    }

    m_speed = speed;
    setDirection();
    return true;
}

void Car::setDirection()
{
    if (m_speed == MIN_SPEED)
    {
        m_direction = Direction::STANDING_STILL;
    }
    else if (m_gear == MIN_GEAR && m_speed != MIN_SPEED)
    {
        m_direction = Direction::BACKWARD;
    }
    else if (m_gear != MIN_GEAR && m_speed != MIN_SPEED)
    {
        m_direction = Direction::FORWARD;
    }
}
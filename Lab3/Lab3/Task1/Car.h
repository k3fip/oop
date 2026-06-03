#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>

const int MIN_GEAR = -1;
const int MAX_GEAR = 5;
const int MIN_SPEED = 0;
const int MAX_SPEED = 150;


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
	bool GetEngine() const { return m_isTurnedOn; };
	int GetSpeed() const { return m_speed; };
	int GetGear() const { return m_gear; };
	std::string GetDirection() const { return m_direction; };

	bool TurnOnEngine()
	{
		if (m_gear == 0 && m_speed == MIN_SPEED)
		{
			m_isTurnedOn = true;
			return true;
		}
		else
		{
			std::cout << "Car must be stopped and in neutral gear" << std::endl;
			return false;
		}
	}

	bool TurnOffEngine()
	{
		if (m_gear == 0 && m_speed == MIN_SPEED)
		{
			m_isTurnedOn = false;
			return true;
		}
		else
		{
			std::cout << "Car must be stopped and in neutral gear" << std::endl;
			return false;
		}
	}

	bool SetGear(int gear)
	{
		if (gear != 0 && m_isTurnedOn == false)
		{
			std::cout << "Cannot set gear while engine is off" << std::endl;
			return false;
		}

		if (gear < MIN_GEAR || gear > MAX_GEAR)
		{
			std::cout << "Invalid gear" << std::endl;
			return false;
		}

		if (gear == MIN_GEAR && m_speed != MIN_SPEED)
		{
			std::cout << "Cannot reverse while moving" << std::endl;
			return false;
		}

		if (speedLimits[gear].first > m_speed || speedLimits[gear].second < m_speed || m_direction == "backward")
		{
			std::cout << "Unsuitable current speed" << std::endl;
			return false;
		}

		m_gear = gear;
		return true;
	}

	bool SetSpeed(int speed)
	{
		if (m_isTurnedOn == false)
		{
			std::cout << "Cannot set speed while engine is off" << std::endl;
			return false;
		}

		if (speed < MIN_SPEED || speed > MAX_SPEED)
		{
			std::cout << "Speed must be in [ 0; 150 ]" << std::endl;
			return false;
		}

		if (m_gear == 0 && speed > m_speed)
		{
			std::cout << "Cannot accelerate on neutral" << std::endl;
			return false;
		}

		if (speedLimits[m_gear].first > speed || speedLimits[m_gear].second < speed)
		{
			std::cout << "Speed is out of gear range" << std::endl;
			return false;
		}

		m_speed = speed;
		setDirection();
		return true;
	}

private:

	bool m_isTurnedOn = false;
	int m_gear = 0;
	int m_speed = MIN_SPEED;
	std::string m_direction = "standing still";

	void setDirection()
	{
		if (m_speed == MIN_SPEED) { m_direction = "standing still"; }
		else if (m_gear == MIN_GEAR && m_speed != MIN_SPEED) { m_direction = "backward"; }
		else if (m_gear != MIN_GEAR && m_speed != MIN_SPEED) { m_direction = "forward"; }
	}

};

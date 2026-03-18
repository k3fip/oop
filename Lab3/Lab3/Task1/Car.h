#pragma once

#include <string>

class Car
{
public:
	bool GetEngine() const { return m_isTurnedOn; };
	int GetSpeed() const { return m_speed; };
	int GetGear() const { return m_gear; };
	std::string GetDirection() const { return m_direction; };

	bool TurnOnEngine()
	{
		if (m_gear == 0 && m_speed == 0)
		{
			m_isTurnedOn = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool TurnOffEngine()
	{
		if (m_gear == 0 && m_speed == 0)
		{
			m_isTurnedOn = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SetGear(int gear)
	{
		if (gear == 0)
		{
			m_gear = 0;
			return true;
		}

		if (m_isTurnedOn == false)
		{
			return false;
		}

		switch (gear)
		{
		case -1:
			if (m_speed != 0) { return false; };
			break;

		case 1:
			if (m_speed > 30 || m_direction == "backward") { return false; };
			break;

		case 2:
			if (m_speed > 50 || m_speed < 20 || m_direction == "backward") { return false; };
			break;

		case 3:
			if (m_speed > 60 || m_speed < 30 || m_direction == "backward") { return false; };
			break;

		case 4:
			if (m_speed > 90 || m_speed < 40 || m_direction == "backward") { return false; };
			break;

		case 5:
			if (m_speed > 150 || m_speed < 50 || m_direction == "backward") { return false; };
			break;

		default:
			return false;
			break;

		}

		m_gear = gear;
		return true;
	}

	bool SetSpeed(int speed)
	{
		switch (m_gear)
		{
		case -1:
			if (speed < 0 || speed > 20) { return false; }
			break;

		case 0:
			if (speed < 0 || speed > m_speed) { return false; }
			break;

		case 1:
			if (speed < 0 || speed > 30) { return false; }
			break;

		case 2:
			if (speed < 20 || speed > 50) { return false; }
			break;

		case 3:
			if (speed < 30 || speed > 60) { return false; }
			break;

		case 4:
			if (speed < 40 || speed > 90) { return false; }
			break;

		case 5:
			if (speed < 50 || speed > 150) { return false; }
			break;
		}
		m_speed = speed;
		setDirection();
		return true;
	}

private:

	bool m_isTurnedOn = false;
	int m_gear = 0;
	int m_speed = 0;
	std::string m_direction = "standing still";

	void setDirection()
	{
		if (m_speed == 0) { m_direction = "standing still"; }
		else if (m_gear == -1 && m_speed != 0) { m_direction = "backward"; }
		else if (m_gear != -1 && m_speed != 0) { m_direction = "forward"; }
	}

};

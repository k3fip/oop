#pragma once
#include <iomanip>
#include <stdexcept>

constexpr unsigned SECONDS_IN_MINUTE = 60;
constexpr unsigned MINUTES_IN_HOUR = 60;
constexpr unsigned HOURS_IN_DAY = 24;
constexpr unsigned SECONDS_IN_HOUR = SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
constexpr unsigned SECONDS_IN_DAY = SECONDS_IN_HOUR * HOURS_IN_DAY;
constexpr unsigned MAX_TIMESTAMP = SECONDS_IN_DAY - 1;
constexpr unsigned MAX_HOURS = HOURS_IN_DAY - 1;
constexpr unsigned MAX_MINUTES = MINUTES_IN_HOUR - 1;
constexpr unsigned MAX_SECONDS = SECONDS_IN_MINUTE - 1;
constexpr char TIME_SEPARATOR = ':';
constexpr unsigned OUTPUT_WIDTH = 2;
constexpr char PADDING_CHAR = '0';

class CTime
{
public:
    CTime(unsigned timeStamp = 0);
    CTime(unsigned hours, unsigned minutes, unsigned seconds = 0);

    unsigned GetHours() const;
    unsigned GetMinutes() const;
    unsigned GetSeconds() const;

    CTime& operator++();
    CTime operator++(int);
    CTime& operator--();
    CTime operator--(int);
    CTime operator+(const CTime& other) const;
    CTime operator-(const CTime& other) const;
    CTime& operator+=(const CTime& other);
    CTime& operator-=(const CTime& other);
    CTime operator*(unsigned multiplier) const;
    CTime operator/(unsigned divisor) const;
    unsigned operator/(const CTime& other) const;
    CTime& operator*=(unsigned multiplier);
    CTime& operator/=(unsigned divisor);
    bool operator==(const CTime& other) const;
    bool operator!=(const CTime& other) const;
    bool operator<(const CTime& other) const;
    bool operator>(const CTime& other) const;
    bool operator<=(const CTime& other) const;
    bool operator>=(const CTime& other) const;

private:
    unsigned m_seconds;

};

CTime operator*(unsigned multiplier, const CTime& time);

std::ostream& operator<<(std::ostream& os, const CTime& time);
std::istream& operator>>(std::istream& is, CTime& time);
#include "CTime.h"

CTime::CTime(unsigned timeStamp) : m_seconds(timeStamp% SECONDS_IN_DAY)
{
    if (timeStamp > MAX_TIMESTAMP)
    {
        m_seconds = 0;
        throw std::logic_error("");
    }
}

CTime::CTime(unsigned hours, unsigned minutes, unsigned seconds)
    : CTime(hours* SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds)
{
    if (hours > MAX_HOURS || minutes > MAX_MINUTES || seconds > MAX_SECONDS)
    {
        m_seconds = 0;
        throw std::logic_error("");
    }
}

unsigned CTime::GetHours() const
{
    return m_seconds / SECONDS_IN_HOUR;
}

unsigned CTime::GetMinutes() const
{
    return (m_seconds % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
}

unsigned CTime::GetSeconds() const
{
    return m_seconds % SECONDS_IN_MINUTE;
}

CTime& CTime::operator++()
{
    m_seconds = (m_seconds + 1) % SECONDS_IN_DAY;
    return *this;
}

CTime CTime::operator++(int)
{
    CTime temp(*this);
    ++(*this);
    return temp;
}

CTime& CTime::operator--()
{
    m_seconds = (m_seconds + MAX_TIMESTAMP) % SECONDS_IN_DAY;
    return *this;
}

CTime CTime::operator--(int)
{
    CTime temp(*this);
    --(*this);
    return temp;
}

CTime CTime::operator+(const CTime& other) const
{
    return CTime((m_seconds + other.m_seconds) % SECONDS_IN_DAY);
}

CTime CTime::operator-(const CTime& other) const
{
    return CTime((m_seconds + SECONDS_IN_DAY - other.m_seconds) % SECONDS_IN_DAY);
}

CTime& CTime::operator+=(const CTime& other)
{
    m_seconds = (m_seconds + other.m_seconds) % SECONDS_IN_DAY;
    return *this;
}

CTime& CTime::operator-=(const CTime& other)
{
    m_seconds = (m_seconds + SECONDS_IN_DAY - other.m_seconds) % SECONDS_IN_DAY;
    return *this;
}

CTime CTime::operator*(unsigned multiplier) const
{
    unsigned long long result = static_cast<unsigned long long>(m_seconds) * multiplier;
    return CTime(static_cast<unsigned>(result % SECONDS_IN_DAY));
}

CTime operator*(unsigned multiplier, const CTime& time)
{
    return time * multiplier;
}

CTime CTime::operator/(unsigned divisor) const
{
    if (divisor == 0)
    {
        throw CTime(MAX_TIMESTAMP);
    }
    return CTime(m_seconds / divisor);
}

unsigned CTime::operator/(const CTime& other) const
{
    if (other.m_seconds == 0)
    {
        return MAX_TIMESTAMP;
    }
    return m_seconds / other.m_seconds;
}

CTime& CTime::operator*=(unsigned multiplier)
{
    *this = *this * multiplier;
    return *this;
}

CTime& CTime::operator/=(unsigned divisor)
{
    *this = *this / divisor;
    return *this;
}

bool CTime::operator==(const CTime& other) const
{
    return m_seconds == other.m_seconds;
}

bool CTime::operator!=(const CTime& other) const
{
    return !(*this == other);
}

bool CTime::operator<(const CTime& other) const
{
    return m_seconds < other.m_seconds;
}

bool CTime::operator>(const CTime& other) const
{
    return m_seconds > other.m_seconds;
}

bool CTime::operator<=(const CTime& other) const
{
    return m_seconds <= other.m_seconds;
}

bool CTime::operator>=(const CTime& other) const
{
    return m_seconds >= other.m_seconds;
}

std::ostream& operator<<(std::ostream& os, const CTime& time)
{
    os << std::setfill(PADDING_CHAR) << std::setw(OUTPUT_WIDTH) << time.GetHours() << TIME_SEPARATOR
        << std::setfill(PADDING_CHAR) << std::setw(OUTPUT_WIDTH) << time.GetMinutes() << TIME_SEPARATOR
        << std::setfill(PADDING_CHAR) << std::setw(OUTPUT_WIDTH) << time.GetSeconds();
    return os;
}

std::istream& operator>>(std::istream& is, CTime& time)
{
    unsigned hours, minutes, seconds;
    char colon1, colon2;

    std::streampos pos = is.tellg();

    if (is >> hours >> colon1 >> minutes >> colon2 >> seconds)
    {
        if (colon1 != TIME_SEPARATOR || colon2 != TIME_SEPARATOR)
        {
            is.clear();
            is.seekg(pos);
        }
        else
        {
            try
            {
                time = CTime(hours, minutes, seconds);
                return is;
            }
            catch (const std::logic_error&)
            {
                is.setstate(std::ios::failbit);
                return is;
            }
        }
    }
    else
    {
        is.clear();
        is.seekg(pos);
    }

    unsigned timestamp;
    if (is >> timestamp)
    {
        try
        {
            time = CTime(timestamp);
            return is;
        }
        catch (const std::logic_error&)
        {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    is.setstate(std::ios::failbit);
    return is;
}
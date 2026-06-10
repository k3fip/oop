#include "SortStrings3.h"

void SortStrings3(std::string& s1, std::string& s2, std::string& s3) noexcept
{
    if (s2 < s1) {
        std::swap(s1, s2);
    }
    if (s3 < s2) {
        std::swap(s2, s3);
        if (s2 < s1) {
            std::swap(s1, s2);
        }
    }
}
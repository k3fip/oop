#include <cstring>

template <typename T>
void Sort2(T& first, T& second) {
    if (first > second) {
        T temp = first;
        first = second;
        second = temp;
    }
}

template <>
void Sort2<const char*>(const char*& first, const char*& second) {
    if (std::strcmp(first, second) > 0) {
        const char* temp = first;
        first = second;
        second = temp;
    }
}
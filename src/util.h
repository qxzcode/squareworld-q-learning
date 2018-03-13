#pragma once

#include <cstdint>
using std::uint64_t;
using std::uint32_t;
using std::uint8_t;

namespace util {
    
    double rand();
    
    double getTime();
    
    void sleep(long ms);

    bool compareDouble(const double a, const double b);
    
    template<class T, unsigned long N>
    constexpr auto arraySize(const T(&)[N]) {
        return N;
    }
    
}
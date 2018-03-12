#pragma once

namespace util {
    
    double rand();
    
    double getTime();
    
    void sleep(long ms);
    
    template<class T, unsigned long N>
    constexpr auto arraySize(const T(&)[N]) {
        return N;
    }
    
}
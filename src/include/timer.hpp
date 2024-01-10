#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <string>

class Timer {
public:
    static std::map<std::string, std::chrono::steady_clock::time_point> timers;    
    static void start(std::string);
    static long stop(std::string);
};

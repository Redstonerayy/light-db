#include "timer.hpp"

// initialize
std::map<std::string, std::chrono::steady_clock::time_point> Timer::timers = std::map<std::string, std::chrono::steady_clock::time_point>();

void Timer::start(std::string name){
    auto timenow = std::chrono::steady_clock::now();
    timers[name] = timenow;
}

long Timer::stop(std::string name){
    auto timenow = std::chrono::steady_clock::now();
    auto diffns = std::chrono::duration_cast<std::chrono::nanoseconds>(timenow - timers[name]);
    auto diffms = std::chrono::duration_cast<std::chrono::milliseconds>(timenow - timers[name]);
    auto diffs = std::chrono::duration_cast<std::chrono::duration<double>>(timenow - timers[name]);
    std::cout << name << " took " << diffns.count() << " Nanoseconds, " << diffms.count() << " Miliseconds," << std::endl;
    return diffns.count();
}

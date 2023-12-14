#include <thread>
#include <chrono>

#include "util.hpp"

namespace util
{

unsigned int get_logical_cpus()
{
    const unsigned int logical_cpus = std::thread::hardware_concurrency(); 
    return logical_cpus != 0 ? logical_cpus : 1; // return 1 if can't detect count
}

}

//
//  hires_timer.cpp
//  CppPractice
//
//  Created by Pavel Sh on 20.12.2023.
//
#include "timer.h"

using std::operator""s;

MsecTimer::MsecTimer (std::string file, int line, std::string message)
: message_(message) {
    start_ = std::chrono::steady_clock::now();
    if(message_.empty())
    {
        message_ = file + ':' + std::to_string(line);
    }
}

MsecTimer::~MsecTimer() {
    finish_ = std::chrono::steady_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(finish_ - start_);
    std::cout << "SteadyTimer |"s << message_ << ": "s << time_span.count() << " msec."s << std::endl;
}



HiResTimer::HiResTimer (std::string file, int line, std::string message)
: message_(message)
{
    start_ = std::chrono::high_resolution_clock::now();
    if(message_.empty())
    {
        message_ = file + ':' + std::to_string(line);
    }
}

HiResTimer::~HiResTimer()
{
    finish_ = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_ - start_);
    std::cout << "HiResTimer|"s << message_ << ": "s << time_span.count() << "ns"s << std::endl;
}

//
//  log_duration.h
//  CppPractice
//
//  Created by Pavel Sh on 21.12.2023.
//
#pragma once

#include <chrono>
#include <iostream>
#include <string>

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;
    
    LogDuration() {}
    
    explicit LogDuration(const std::string& message)
    : message_(message) {
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;
       
        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        if(!message_.empty()) {
            std::cerr << message_ << ": "s;
        }
        std::cerr << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
   }

private:
    const Clock::time_point start_time_ = Clock::now();
    std::string message_;
};

#define LOG_DURATION(m) { LogDuration((m)); };

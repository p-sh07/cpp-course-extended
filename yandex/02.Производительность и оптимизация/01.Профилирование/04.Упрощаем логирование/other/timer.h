//
//  hires_timer.hpp
//  CppPractice
//
//  Created by Pavel Sh on 20.12.2023.
//
#pragma once

#include <iostream>
#include <string>
#include <chrono>

class MsecTimer
{
public:
    MsecTimer(std::string file = "", int line = -1, std::string message = "");
    ~MsecTimer();
private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point finish_;
    std::string message_;

};

class HiResTimer
{
public:
    HiResTimer(std::string file = "", int line = -1, std::string message = "");
    ~HiResTimer();
private:
    std::chrono::high_resolution_clock::time_point start_;
    std::chrono::high_resolution_clock::time_point finish_;
    std::string message_="";
};

#define HIRES_TIMER(m) { HiResTimer(__FILE__, __LINE__, (m)); };

#define MSEC_TIMER(m) { MsecTimer(__FILE__, __LINE__, (m)); };

//
//  05.Производительность и оптимизация
//  Тема 1.Профилируем и ускоряем
//  Задача 05.Используем макрос
//
//  Created by Pavel Sh on 23.12.2023.
//

#pragma once

#include <chrono>
#include <iostream>
#include <string>

#define UNIQUE_ID_IMPL(lineno) profile_guard_##lineno
#define UNIQUE_ID(lineno) UNIQUE_ID_IMPL(lineno)
#define LOG_DURATION(message) LogDuration UNIQUE_ID(__LINE__){message};


class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string& id, std::ostream& dst_stream = std::cerr)
        : id_(id)
        , dst_stream_(dst_stream) {
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        dst_stream_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
    std::ostream& dst_stream_;
};



//
//  03.Производительность и оптимизация
//  Тема 01.Ассоциативные контейнеры
//  Урок 05.Unordered map - part 2
//
//  Created by Pavel Sh on 26.02.2024
//
#include "log_duration.h"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;

class VehiclePlate {
private:
    auto AsTuple() const {
        return tie(letters_, digits_, region_);
    }

public:
    bool operator==(const VehiclePlate& other) const {
        return AsTuple() == other.AsTuple();
    }

    bool operator<(const VehiclePlate& other) const {
        return AsTuple() < other.AsTuple();
    }

    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{l0, l1, l2}
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    const array<char, 3>& GetLetters() const {
        return letters_;
    }

    int GetDigits() const {
        return digits_;
    }

    int GetRegion() const {
        return region_;
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

struct PlateHasherTrivial {
    size_t operator()(const VehiclePlate& plate) const {
        return static_cast<size_t>(plate.GetDigits());
    }
};

struct PlateHasherRegion {
    size_t operator()(const VehiclePlate& plate) const {
        return static_cast<size_t>(plate.GetDigits() + plate.GetRegion() * 1000);
    }
};

struct PlateHasherString {
    size_t operator()(const VehiclePlate& plate) const {
        return hasher(plate.ToString());
    }

    hash<string> hasher;
};

struct PlateHasherAll {
    uint64_t operator()(const VehiclePlate& plate) const {
        
        std::hash<int> ih;
    
        auto letters = plate.GetLetters();
        uint64_t n = 37; // n^2 * hash1 + n * hash2 + hash3;
        uint64_t letters_hash = n*n*(letters[0]-'A') + n*(letters[1]-'A') + letters[2]-'A';
        // достаточно большое для избежания коллизий простое число
        return n*n*ih(plate.GetDigits()) + n*letters_hash + plate.GetRegion();
    }
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

class PlateGenerator {
    char GenerateChar() {
        uniform_int_distribution<short> char_gen{0, static_cast<short>(possible_chars_.size() - 1)};
        return possible_chars_[char_gen(engine_)];
    }

    int GenerateNumber() {
        uniform_int_distribution<short> num_gen{0, 999};
        return num_gen(engine_);
    }

    int GenerateRegion() {
        uniform_int_distribution<short> region_gen{0, static_cast<short>(possible_regions_.size() - 1)};
        return possible_regions_[region_gen(engine_)];
    }

public:
    VehiclePlate Generate() {
        return VehiclePlate(GenerateChar(), GenerateChar(), GenerateNumber(), GenerateChar(), GenerateRegion());
    }

private:
    mt19937 engine_;

    // допустимые значения сохраним в static переменных
    // они объявлены inline, чтобы их определение не надо было выносить вне класса
    inline static const array possible_regions_
        = {1,  2,  102, 3,   4,   5,   6,   7,   8,  9,   10,  11,  12, 13,  113, 14,  15, 16,  116, 17, 18,
           19, 20, 21,  121, 22,  23,  93,  123, 24, 84,  88,  124, 25, 125, 26,  27,  28, 29,  30,  31, 32,
           33, 34, 35,  36,  136, 37,  38,  85,  39, 91,  40,  41,  82, 42,  142, 43,  44, 45,  46,  47, 48,
           49, 50, 90,  150, 190, 51,  52,  152, 53, 54,  154, 55,  56, 57,  58,  59,  81, 159, 60,  61, 161,
           62, 63, 163, 64,  164, 65,  66,  96,  67, 68,  69,  70,  71, 72,  73,  173, 74, 174, 75,  80, 76,
           77, 97, 99,  177, 199, 197, 777, 78,  98, 178, 79,  83,  86, 87,  89,  94,  95};

    // постфикс s у литерала тут недопустим, он приведёт к неопределённому поведению
    inline static const string_view possible_chars_ = "ABCEHKMNOPTXY"sv;
};

int main() {
    static const int N = 1'000'000;

    PlateGenerator generator;
    vector<VehiclePlate> fill_vector;
    vector<VehiclePlate> find_vector;

    generate_n(back_inserter(fill_vector), N, [&]() {
        return generator.Generate();
    });
    generate_n(back_inserter(find_vector), N, [&]() {
        return generator.Generate();
    });

    int found;
    {
        LOG_DURATION("unordered_set");
        unordered_set<VehiclePlate, PlateHasherAll> container;
        for (auto& p : fill_vector) {
            container.insert(p);
        }
        found = count_if(find_vector.begin(), find_vector.end(), [&](const VehiclePlate& plate) {
            return container.count(plate) > 0;
        });
    }
    cout << "Found matches (1): "s << found << endl;

    {
        LOG_DURATION("set");
        set<VehiclePlate> container;
        for (auto& p : fill_vector) {
            container.insert(p);
        }
        found = count_if(find_vector.begin(), find_vector.end(), [&](const VehiclePlate& plate) {
            return container.count(plate) > 0;
        });
    }
    cout << "Found matches (2): "s << found << endl;
}

/*
#include <array>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;


class VehiclePlate {
public:
    
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
    : letters_{l0, l1, l2}
    , digits_(digits)
    , region_(region) {
    }
    
    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;
        
        return out.str();
    }
    
    auto AsTuple() const {
        return tie(letters_, digits_, region_);
    }
    
    bool operator==(const VehiclePlate& other) const {
        return AsTuple() == other.AsTuple();
    }
    
private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

// возьмите реализацию хешера из прошлого задания
class VehiclePlateHasher {
public:
    size_t operator()(const VehiclePlate& plate) const {
        std::hash<string> str_hasher;
        return str_hasher(plate.ToString());
    }
};


// выбросьте это исключение в случае ошибки парковки
struct ParkingException {};

template <typename Clock>
class Parking {
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;
    
    using CostMap = unordered_map<VehiclePlate, int64_t, PlateHasherAll>;
    
public:
    Parking(int cost_per_second) : cost_per_second_(cost_per_second) {}
    
    // запарковать машину с указанным номером
    void Park(VehiclePlate car) {
        // машина уже запаркована, ошибка
        if(now_parked_.count(car) != 0) {
            throw ParkingException();
        }
        now_parked_[car] = Clock::now();
    }
    
    // забрать машину с указанным номером
    void Withdraw(const VehiclePlate& car) {
        // машины нет на парковке, ошибка
        if(now_parked_.count(car) == 0) {
            throw ParkingException();
        }
        complete_parks_[car] += Clock::now() - now_parked_.at(car);
        now_parked_.erase(car);
    }
    
    // получить счёт за конкретный автомобиль
    int64_t GetCurrentBill(const VehiclePlate& car) const {
        // таймер не сбрасывается
        Duration result{0};
        
        if(complete_parks_.count(car) != 0) {
            // Есть в базе
            result += complete_parks_.at(car);
        }
        if(now_parked_.count(car) != 0) {
            // запаркована сейчас
            result += Clock::now() - now_parked_.at(car);
        } // вернуть стоимость
        return CastToSeconds(result) * cost_per_second_;
    }
    
    // завершить расчётный период
    CostMap EndPeriodAndGetBills() {
        CostMap bill;
        auto now = Clock::now();
        // цикл по припаркованым мащинам
        for(auto& [car, time_when_parked] : now_parked_) {
            // Посчитать текущую стоимость со сбросом таймера
            AddParkingCost(bill, car, {now - time_when_parked});
            time_when_parked = now;
        }
        // цикл по завершенным парковкам
        for(const auto& [car, duration_at_parking] : complete_parks_) {
            AddParkingCost(bill, car, duration_at_parking);
        } 
        // обнулить все счета
        complete_parks_.clear();
        return bill;
    }
    
    auto& GetNowParked() const {
        return now_parked_;
    }
    
    auto& GetCompleteParks() const {
        return complete_parks_;
    }
    
private:
    int64_t CastToSeconds(const Duration& dur) const {
        using namespace chrono;
        return duration_cast<seconds>(dur).count();
    }
    
    void AddParkingCost(CostMap& costs, const VehiclePlate& car, Duration parking_duration) {
        auto seconds_parked = CastToSeconds(parking_duration);
        // не добавлять нулевые результаты
        if(seconds_parked > 0) {
            costs[car] += seconds_parked * cost_per_second_;
        }
    }
    
    int cost_per_second_;
    unordered_map<VehiclePlate, TimePoint, PlateHasherAll> now_parked_;
    unordered_map<VehiclePlate, Duration, PlateHasherAll> complete_parks_;
};

// эти часы удобно использовать для тестирования
// они покажут столько времени, сколько вы задали явно
class TestClock {
public:
    using time_point = chrono::system_clock::time_point;
    using duration = chrono::system_clock::duration;
    
    static void SetNow(int seconds) {
        current_time_ = seconds;
    }
    
    static time_point now() {
        return start_point_ + chrono::seconds(current_time_);
    }
    
private:
    inline static time_point start_point_ = chrono::system_clock::now();
    inline static int current_time_ = 0;
};

int main() {
    Parking<TestClock> parking(10);
    
    TestClock::SetNow(10);
    parking.Park({'A', 'A', 111, 'A', 99});
    
    TestClock::SetNow(20);
    parking.Withdraw({'A', 'A', 111, 'A', 99});
    parking.Park({'B', 'B', 222, 'B', 99});
    
    TestClock::SetNow(40);
    assert(parking.GetCurrentBill({'A', 'A', 111, 'A', 99}) == 100);
    assert(parking.GetCurrentBill({'B', 'B', 222, 'B', 99}) == 200);
    parking.Park({'A', 'A', 111, 'A', 99});
    
    TestClock::SetNow(50);
    assert(parking.GetCurrentBill({'A', 'A', 111, 'A', 99}) == 200);
    assert(parking.GetCurrentBill({'B', 'B', 222, 'B', 99}) == 300);
    assert(parking.GetCurrentBill({'C', 'C', 333, 'C', 99}) == 0);
    parking.Withdraw({'B', 'B', 222, 'B', 99});
    
    TestClock::SetNow(70);
    {
        // проверим счёт
        auto bill = parking.EndPeriodAndGetBills();
        
        // так как внутри макроса используется запятая,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
                == unordered_map<VehiclePlate, int64_t, PlateHasherAll>{
            {{'A', 'A', 111, 'A', 99}, 400},
            {{'B', 'B', 222, 'B', 99}, 300},
        }));
    }
    
    TestClock::SetNow(80);
    {
        // проверим счёт
        auto bill = parking.EndPeriodAndGetBills();
        
        // так как внутри макроса используется запятая,
        // нужно заключить его аргумент в дополнительные скобки
        assert((bill
                == unordered_map<VehiclePlate, int64_t, PlateHasherAll>{
            {{'A', 'A', 111, 'A', 99}, 100},
        }));
    }
    
    try {
        parking.Park({'A', 'A', 111, 'A', 99});
        assert(false);
    }
    catch (ParkingException) {
    }
    
    try {
        parking.Withdraw({'B', 'B', 222, 'B', 99});
        assert(false);
    }
    catch (ParkingException) {
    }
    
    cout << "Success!"s << endl;
}
*/

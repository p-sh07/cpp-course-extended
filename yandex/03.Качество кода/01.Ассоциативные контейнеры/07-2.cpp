//
//  03.Производительность и оптимизация
//  Тема 01.Ассоциативные контейнеры
//  Урок 07-2.Искусство хэш-функций
//
//  Created by Pavel Sh on 27.02.2024
//
#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHash {
    size_t operator() (const Circle& c) {
        double n = 769;
        return static_cast<size_t>(n*n*c.x + n*c.y + c.r);
    }
};

struct DumbbellHash {
// реализуйте хешер для Dumbbell
    size_t operator() (const Dumbbell& d) {
        int64_t n = 3079;
        CircleHash ch;
        std::hash<string> sh;
        return static_cast<size_t>(n*n*ch(d.circle1) + n*ch(d.circle2) + sh(d.text));
    }
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}

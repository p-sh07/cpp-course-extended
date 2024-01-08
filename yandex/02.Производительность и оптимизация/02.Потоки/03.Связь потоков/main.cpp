//
//  05.Производительность и оптимизация
//  Тема 2.Профилируем и ускоряем
//  Задача 04.Связь потоков: в поисках  оптимального ввода-вывода
//
//  Created by Pavel Sh on 24.12.2023.
//
#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    StreamUntier(istream& stream) : stream_(stream) {
        tied_before_ = stream_.tie(nullptr);
    }
    ~StreamUntier() {
        stream_.tie(tied_before_);
    }
    // и дополнительные поля класса при необходимости
private:
    istream& stream_;
    ostream* tied_before_;
};

int main() {
    LOG_DURATION("\\n with tie"s);
    
    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}

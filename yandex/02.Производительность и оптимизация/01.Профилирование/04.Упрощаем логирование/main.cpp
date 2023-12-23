//
//  05.Производительность и оптимизация
//  Тема 1.Профилируем и ускоряем
//  Задача 04.Упрощаем логирование
//
//  Created by Pavel Sh on 18.12.2023.
//
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }
    
    return res;
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;
    
    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }
    
    return res;
}

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // с помощью (rand() % 2) получим целое число в диапазоне 0..1.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

void Operate() {
    LogDuration log("Total");
    vector<int> random_bits;
    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;
    
    // заполним вектор случайными числами 0 и 1
    {
        LogDuration log("Append random");
        AppendRandom(random_bits, N);
    }
    // перевернём вектор задом наперёд
    vector<int> reversed_bits;
    {
        LogDuration log("Reverse");
        reversed_bits = ReverseVector(random_bits);
        
    }
    {
        LogDuration log("Counting");
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
    return 0;
}

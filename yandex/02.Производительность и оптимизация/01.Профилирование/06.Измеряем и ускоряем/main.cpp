//
//  05.Производительность и оптимизация
//  Тема 1.Профилируем и ускоряем
//  Задача 06.Измеряем и ускоряем
//
//  Created by Pavel Sh on 18.12.2023.
//
#include "log_duration.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    LOG_DURATION("Naive");
    vector<int> res;

    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

vector<int> ReverseVector2(const vector<int>& source_vector) {
    LOG_DURATION("Good");
    vector<int> res;

    // будем проходить source_vector задом наперёд
    // с помощью обратного итератора
    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator) {
        res.push_back(*iterator);
    }

    return res;
}

vector<int> ReverseVector3(const vector<int>& source_vector) {
    LOG_DURATION("Best");
    return {source_vector.rbegin(), source_vector.rend()};
}

vector<int> ReverseVector4(const vector<int>& source_vector) {
    LOG_DURATION("Your");
    vector<int> res;
    res.reserve(source_vector.size());
    // реализация вашего собственного реверсирования
    for(auto rIt = source_vector.rbegin(); rIt != source_vector.rend(); ++rIt) {
        res.push_back(*rIt);
    }

    return res;
}

void Operate() {
    vector<int> rand_vector;
    int n;

    cin >> n;
    rand_vector.reserve(n);

    for (int i = 0; i < n; ++i) {
        rand_vector.push_back(rand());
    }

    // код измерения тут
    if(n <= 100000) {
        ReverseVector(rand_vector);
        ReverseVector2(rand_vector);
    } else {
        ReverseVector2(rand_vector);
        ReverseVector3(rand_vector);
        ReverseVector4(rand_vector);
    }
}

int main() {
    Operate();
    return 0;
}

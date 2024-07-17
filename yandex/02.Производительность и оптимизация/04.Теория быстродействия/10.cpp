//
//  05.Производительность и оптимизация
//  Тема 4.Теория сложности
//  Задача 10.
//
//  Created by Pavel Sh on 15.01.2024.
//
//
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

/*
 Пусть дан вектор из 10000 элементов с числами от 0 до 4999. Требуется найти количество чисел, не превышающих 5. Так как возможных чисел 5000, каждое число встречается в среднем 2 раза. Ожидаемый ответ — 12, так как нас устраивают 6 чисел. Двоичный логарифм числа 10000 примерно равен 13,3, так что выбираем find_if.
 Если требуется найти числа до 100, то ожидаемый ответ 202 и мы выбираем upper_bound.
 */

int EffectiveCount(const vector<int>& v, int n, int i) {
    // место для вашего решения
    int expected_answer = static_cast<int>(i * (v.size()*1.0/n));
    int log_size = static_cast<int>(log2(v.size()*1.0));
    
    if(expected_answer <= log_size) {
        cout << "Using find_if" << endl;
        auto it = std::find_if(v.begin(), v.end(), [i](const int num) {
            return num > i;
        });
        return static_cast<int>(it - v.begin());
    } else {
        cout << "Using upper_bound" << endl;
        return static_cast<int>(upper_bound(v.begin(), v.end(), i) - v.begin());
    }
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}

//
//  05.Производительность и оптимизация
//  Тема 1.Профилируем и ускоряем
//  Задача 09.Вглубь процессора (оптимизация)
//
//  Created by Pavel Sh on 23.12.2023.
//

#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    // место для вашего решения
    if(measures.empty()) {
        return {};
    }
    vector<float> temps_sum(measures[0].size(), 0);
    vector<float> temps_number(measures[0].size(), 0);
        
    for(const auto& day : measures) {
        for(size_t i = 0; i < day.size(); ++i) {
            temps_sum[i] += day[i] < 0 ? 0 : day[i];
            temps_number[i] += day[i] < 0 ? 0 : 1;
        }
    }
    for(size_t j = 0; j < temps_sum.size(); ++j) {
        temps_sum[j] /= temps_sum[j] == 0 ? 1 : temps_number[j];
    }
    return temps_sum;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, -1);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}

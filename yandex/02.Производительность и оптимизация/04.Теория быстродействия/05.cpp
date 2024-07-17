//
//  05.Производительность и оптимизация
//  Тема 4.Теория сложности
//  Задача 05. Улучшаем сложность
//
//  Created by Pavel Sh on 14.01.2024.
//
#include <algorithm>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
template <typename T>
void PrintVecToCerr(const string& vec_name, const vector<T>& vec) {
    cerr << "Printing vec [" << vec_name << "]: ";
    bool is_first = true;
    for(const auto& a : vec) {
        if(!is_first) {
            cerr << ", ";
        }
        is_first = false;
        cerr << a;
    }
    cerr << endl;
}

#define printvec(x) PrintVecToCerr((#x), (x));
// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }
    
// Изначально дано:
//    for (int i : v1) {
//        // проверяем, что каждый элемент первого вектора
//        // содержится одинаковое количество раз в обоих векторах
//        if (count(v1.begin(), v1.end(), i) != count(v2.begin(), v2.end(), i)) {
//            return false;
//        }
//    }
//
//    return true; -> O(N*(N+N)) -> O(N^2)
    
// Ускоренный вариант:
    vector<int> v1_sorted(v1.begin(), v1.end());
    vector<int> v2_sorted(v2.begin(), v2.end());

    sort(v1_sorted.begin(), v1_sorted.end());
    sort(v2_sorted.begin(), v2_sorted.end());

    for(size_t i = 0; i < v1.size(); ++i) {
        if(v1_sorted[i] != v2_sorted[i]) {
            return false;
        }
    }
    return true; //-> O(2*log(N)+N) -> O(N)
}

int main() {
    std::mt19937 g;

    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand()%10);
        v2.push_back(rand()%10);
    }

    // оба вектора случайны, вряд ли они совпадут
    cout << "Random vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    // делаем один перестановкой другого явным образом
    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);

    cout << "Permuted vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}

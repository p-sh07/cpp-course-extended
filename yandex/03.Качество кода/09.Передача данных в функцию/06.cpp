//
//  03.Качество кода
//  Тема 13.Передача данных в функцию
//  Урок 06.Передача набора объектов
//
//  created by ps on 21.05.2024
//
#include <iostream>
#include <list>
#include <set>
#include <string_view>
#include <vector>

using namespace std;
using std::ranges::forward_range;

// Измените сигнатуру этой функции, чтобы она могла
// сливать не только векторы, но и любые другие контейнеры
template <typename It1, typename It2>
void Merge(It1 b1, It1 e1, It2 b2, It2 e2, ostream& out) {
    while (b1 != e1 && b2 != e2) {
        if (std::less{}(*b1, *b2)) {
            out << *b1++ << endl;
        } else {
            out << *b2++ << endl;
        }
    }
    while (b1 != e1) {
        out << *b1++ << endl;
    }
    while (b2 != e2) {
        out << *b2++ << endl;
    }
}

// Реализуйте эти функции. Они должны вызывать ваш вариант функции Merge.
// Чтобы сократить кличество работы, можете реализовать вместо них одну шаблонную.
template <typename Container1, typename Container2>
void MergeSomething(const Container1& src1, const Container2& src2, ostream& out) {
    Merge(src1.begin(), src1.end(), src2.begin(), src2.end(), out);
}

// Реализуйте эту функцию:
template <typename T>
void MergeHalves(span<T> src, ostream& out) {
    // Сделать Merge участка вектора от 0 до mid и от mid до конца.
    // Элемент с индексом mid включается во второй диапазон.
    const size_t mid = (src.size() + 1) / 2;

    span<T> lhs = src.subspan(0, mid);
    span<T> rhs = src.subspan(mid, mid);
    
    MergeSomething(lhs, rhs, out);
}

// Реализуйте эту функцию:
template <typename T>
void MergeHalves(const vector<T>& src, ostream& out) {
    // Сделать Merge участка вектора от 0 до mid и от mid до конца.
    // Элемент с индексом mid включается во второй диапазон.
    const auto mid = src.begin() + (src.size() + 1) / 2;
    
    Merge(src.begin(), mid, mid, src.end(), out);
}

int main() {
    vector<int> v1{60, 70, 80, 90};
    vector<int> v2{65, 75, 85, 95};
    vector<int> combined{60, 70, 80, 90, 65, 75, 85, 95};
    list<double> my_list{0.1, 72.5, 82.11, 1e+30};
    string_view my_string = "ACNZ"sv;
    set<unsigned> my_set{20u, 77u, 81u};

    // пока функция MergeSomething реализована пока только для векторов
    cout << "Merging vectors:"sv << endl;
    MergeSomething(v1, v2, cout);

    cout << "Merging vector and list:"sv << endl;
    MergeSomething(v1, my_list, cout);

    cout << "Merging string and list:"sv << endl;
    MergeSomething(my_string, my_list, cout);

    cout << "Merging set and vector:"sv << endl;
    MergeSomething(my_set, v2, cout);

    cout << "Merging vector halves:"sv << endl;
    MergeHalves(combined, cout);
}

//
//  05.Производительность и оптимизация
//  Тема 4.Теория сложности
//  Задача 06.Логарифмическая сложность
//
//  Created by Pavel Sh on 15.01.2024.
//
#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int n, F drop) {
// Переделайте этот алгоритм, имеющий линейную сложность.
// В итоге должен получится логарифмический алгоритм.
//    for (int i = 1; i < n; ++i) {
//        if (drop(i)) {
//            return i;
//        }
//    }
// My version:
    int top = n, bottom = 0, mid = n/2;
    while(top-bottom > 2) {
        //if true, take lower half, else upper half
        if(drop(mid)) {
            top = mid;
        } else {
            bottom = mid;
        }
        mid = bottom + (top - bottom)/2;
    } //answer is either mid or top
    return drop(mid) ? mid : top;
}

int main() {
    int n,t;
    cout << "Enter n and target floor number: "s << endl;
    cin >> n >> t;

    int count = 0;
    int found = FindFloor(n, [t, &count](int f) {
        ++count;
        return f >= t;
    });

    cout << "Found floor "s << found << " after "s << count << " drops"s << endl;

    return 0;
}

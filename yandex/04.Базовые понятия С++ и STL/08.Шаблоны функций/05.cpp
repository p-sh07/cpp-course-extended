//
//  04.Базовые понятия С++ и STL
//  Тема 08.Шаблоны функций
//  Задача 05.Универсальные функции вывода шаблонов
//
//  Created by Pavel Sh on 28.11.2023.
//  

#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const pair<Key, Value>& Pair) {
    out << '(';
    out << Pair.first << ", " << Pair.second;
    out << ')';
    return out;
}

template<typename TC>
void Print(ostream& out, const TC& container) {
        bool is_first = true;
    for (const auto& element : container) {
        if(!is_first) {
            out <<  ", "s;
        }
        is_first = false;
        out << element;
    }
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    out << '[';
    Print(out, container);
    out << ']';
    return out;
}
template <typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    out << '{';
    Print(out, container);
    out << '}';
    return out;
}
template <typename Key,typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    out << '<';
    Print(out, container);
    out << '>';
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    const map<string, int> stuff = {{"asd",10}, {"req",5}, {"fdd",2}, {"asdf",1},};
    cout << ages << endl;
    cout << stuff << endl;
    return 0;
}

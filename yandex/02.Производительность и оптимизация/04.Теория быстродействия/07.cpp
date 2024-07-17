//
//  05.Производительность и оптимизация
//  Тема 4.Теория сложности
//  Задача 07.Опасности экспоненты
//
//  Created by Pavel Sh on 15.01.2024.
//
// 7.1
#include <cstdint>
#include <iostream>

using namespace std;


// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
// (- пример экспоненты)
//int64_t T(int i) {
//    if (i <= 1) {
//        return 0;
//    }
//    if (i == 2) {
//        return 1;
//    }
//
//    return T(i - 1) + T(i - 2) + T(i - 3);
//}
//Быстрая версия:
int64_t T(int i) {
    int64_t prev0 = 0, prev1 = 0, prev2 = 1;
    if (i <= 1) {
        return prev1;
    }
    if (i == 2) {
        return prev2;
    }
    int64_t result = 1;
    for(int n = 2; n < i; ++n) {
        result = prev0 + prev1 + prev2;
        prev0 = prev1;
        prev1 = prev2;
        prev2 = result;
    }
    return result;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}
//Задание 7.2: BruteForce

#include <iostream>
#include <string>

using namespace std;

void advanceString(string& str) {
    if(str == "ZZZZZ") {
        return;
    } //start at end and replace all Z with A
    size_t pos = str.size()-1;
    while (str[pos] == 'Z') {
        str[pos] = 'A';
        --pos;
    } //now advance the first non-Z symbol
    ++str[pos];
}

template <typename F>
string BruteForce(F check) {
    string password = "AAAAA";
    //start with last letter
    while(password != "ZZZZZ") {
        if(check(password)) {
            return password;
        } else {
            advanceString(password);
        }
    }
    return password;
}

int main() {
    string str = "AAAAA"s;
    while (str != "AAZZZ") {
        advanceString(str);
    }
    
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}



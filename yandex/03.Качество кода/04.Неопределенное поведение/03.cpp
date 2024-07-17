#include <iostream>
#include <string_view>

using namespace std;


// clang-format off
const int NUM_PLANETS = 8;
const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

bool IsPlanet(string_view name) {
    for (int i = 0; i < NUM_PLANETS; ++i) {
        if (PLANETS[i] == name) {
            return true;
        }
    }
    return false;
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv)
        << "a planet"sv << endl;
}

int main() {
//    Test("Earth"sv);
//    Test("Jupiter"sv);
//    Test("Pluto"sv);
//    Test("Moon"sv);
    
    string in_str;
    getline(cin, in_str);
    Test(in_str);
}


//sln:

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>


using namespace std;

// Вместо "сырых" массивов фиксированного размера лучше использовать std::array.
// Тогда можно будет использовать метод size() вместо хранения константы, задающей размер массива
const array PLANETS {
        "Mercury"sv, "Venus"sv, "Earth"sv,
        "Mars"sv, "Jupiter"sv, "Saturn"sv,
        "Uranus"sv, "Neptune"sv,
};

bool IsPlanet(string_view name) {
    // Следует использовать подходящие стандартные алгоритмы, а не писать циклы вручную.
    // Тогда будет меньше вероятность совершить ошибку
    return find(PLANETS.begin(), PLANETS.end(), name) != PLANETS.end();
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv) << "a planet"sv << endl;
}

int main() {
    string name;
    getline(cin, name);
    Test(name);
}

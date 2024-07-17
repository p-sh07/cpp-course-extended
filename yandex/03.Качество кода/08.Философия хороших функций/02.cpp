//
//  03.Качество кода
//  Тема 12.Философия хороших функций
//  Урок 02.Зачем нужен рефакторинг и функции?
// 
//  created by ps on 30.04.2024
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Gender { FEMALE, MALE };

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = range_copy.begin() + range_copy.size() / 2;
    nth_element(range_copy.begin(), middle, range_copy.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}

//employed -> 1, unemployed -> 0, ignore employment -> -1;

using person_it = vector<Person>::iterator;
//using iterator_range = std::pair<person_it, person_it>;

// напишите сигнатуру и реализацию функции PrintStats
void PrintStats(vector<Person>& ppl) {
    //необходимо отсортировать вектор, поэтому либо нужно создать его копию, либо передать по неконастантной ссылке
    cout << "Median age = "s << ComputeMedianAge(ppl.begin(), ppl.end()) << endl;
    
    person_it females_end = std::partition(ppl.begin(), ppl.end(), [](const Person& p) {
        return p.gender == Gender::FEMALE;
    });
    
    cout << "Median age for females = "s << ComputeMedianAge(ppl.begin(), females_end) << endl;
    cout << "Median age for males = "s << ComputeMedianAge(females_end, ppl.end()) << endl;
    
    person_it employed_fem_end = std::partition(ppl.begin(), females_end, [](const Person& p) {
        return p.is_employed == true;
    });
    cout << "Median age for employed females = "s << ComputeMedianAge(ppl.begin(), employed_fem_end) << endl;
    cout << "Median age for unemployed females = "s << ComputeMedianAge(employed_fem_end, females_end) << endl;
    
    person_it employed_male_end = std::partition(females_end, ppl.end(), [](const Person& p) {
        return p.is_employed == true;
    });
    cout << "Median age for employed males = " << ComputeMedianAge(females_end, employed_male_end) << endl;
    cout << "Median age for unemployed males = "s << ComputeMedianAge(employed_male_end, ppl.end()) << endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},   {40, Gender::MALE, false},  {24, Gender::MALE, false},
        {20, Gender::MALE, false},  {80, Gender::MALE, false}, {78, Gender::MALE, false},
        {10, Gender::MALE, false}, {55, Gender::MALE, false},
    };
    PrintStats(persons);
}

/* didn't work ??
 //sort order: female: employed, unemployed,  male: employed, unemployed
 std::sort(ppl.begin(), ppl.end(), [](const Person& lhs, const Person& rhs) {
     return lhs.gender == rhs.gender ? (lhs.is_employed && !rhs.is_employed) : lhs.gender < rhs.gender;
 });
 
 person_it employed_fem_end = std::find_if_not(ppl.begin(), ppl.end(), [](const Person& p) {
     return p.gender == Gender::FEMALE && p.is_employed;
 });
 person_it unemployed_fem_end = std::find_if_not(employed_fem_end, ppl.end(), [](const Person& p) {
     return p.gender == Gender::FEMALE && !p.is_employed;
 });

 person_it employed_male_end = std::find_if_not(unemployed_fem_end, ppl.end(), [](const Person& p) {
     return p.gender == Gender::MALE && p.is_employed;
 });
 
 cout << "Median age = "s << ComputeMedianAge(ppl.begin(), ppl.end()) << endl;
 
 cout << "Median age for females = "s << ComputeMedianAge(ppl.begin(), employed_fem_end) << endl;
 cout << "Median age for males = "s << ComputeMedianAge(unemployed_fem_end, ppl.end()) << endl;
 
 cout << "Median age for employed females = "s << ComputeMedianAge(ppl.begin(), employed_fem_end) << endl;
 cout << "Median age for unemployed females = "s << ComputeMedianAge(employed_fem_end, unemployed_fem_end) << endl;
 cout << "Median age for employed males = " << ComputeMedianAge(unemployed_fem_end, employed_male_end) << endl;
 cout << "Median age for unemployed males = "s << ComputeMedianAge(employed_male_end, ppl.end()) << endl;
}
 */

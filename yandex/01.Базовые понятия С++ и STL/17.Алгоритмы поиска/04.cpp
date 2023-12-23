//
//  04.Базовые понятия С++ и STL
//  Тема 17.Алгоритмы поиска
//  Задача 04.Поиск в отсортированном векторе, словаре и множестве
//
//  Created by Pavel Sh on 16.12.2023.
//

//part 3: sorted vector, string prefix:
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {
    auto lower = lower_bound(range_begin, range_end, prefix);
    //search for next boundary by incrementing last char of prefix
    prefix.back() = static_cast<char>(++prefix.back());
    auto upper = lower_bound(range_begin, range_end, prefix);
    return {lower, upper};
}

int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};
    const auto mo_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;
    const auto mt_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;
    const auto na_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;
    return 0;
}

//part 2: sorted vector, char prefix:
/*
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    auto lower = lower_bound(range_begin, range_end, string{prefix});
    //search for next char
    auto upper = lower_bound(range_begin, range_end, string{static_cast<char>(++prefix)});
    return {lower, upper};
}

int main() {
    const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};
    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;
    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;
    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;
    return 0;
}
*/
//part 1: set
/*
#include <iostream>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    // set<int>::const_iterator — тип итераторов для константного множества целых чисел
    auto lower = numbers.lower_bound(border);
    
    if(lower == numbers.begin()){
        //no smaller elements, or empty
        return lower;
    }
    if(lower == numbers.end()) {
        //set is definitely not empty, so last element is closest
        return prev(lower);
    } //check which of the two is closer
    return border-*prev(lower) <= *lower-border ? prev(lower) : lower;
}

int main() {
    set<int> numbers = {1, 4, 6};
    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
         << *FindNearestElement(numbers, 5) << " "
    << *FindNearestElement(numbers, 6) << " "
         << *FindNearestElement(numbers, 100) << endl;
    set<int> empty_set;
    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}
*/

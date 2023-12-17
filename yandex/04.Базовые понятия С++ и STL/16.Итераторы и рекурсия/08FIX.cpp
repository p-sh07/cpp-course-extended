//
//  04.Базовые понятия С++ и STL
//  Тема 16.Итераторы
//  Задача 08.Стандартные алгоритмы из <algorithm>
//
//  Created by Pavel Sh on 13.12.2023.
//

#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include <stack>
#include <random>

using namespace std;
//Part 1. Permutations

// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    // удобный тип ostringstream -> https://ru.cppreference.com/w/cpp/io/basic_ostringstream
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

template <typename It>
vector<string> GetPermutations(It range_begin, It range_end) {
    vector<string> result;
    //for correct permutations need to sort container
    std::sort(range_begin, range_end);
    do {
        result.push_back((PrintRangeToString(range_begin, range_end)));
    }
    while(next_permutation(range_begin, range_end));
    return result;
}

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << ' ';
    }
    cout << endl;
}

//Part 2. Merge sort
template <typename RandomIt> //[left, right]
void MergeRanges(RandomIt left_it, RandomIt right_it, RandomIt right_end) {
    RandomIt start_of_range = left_it;
    RandomIt left_end = right_it;
    cerr << "Joining ranges ";
    PrintRange(left_it, left_end);
    cerr << " & ";
    PrintRange(right_it, right_end);
    cerr << endl;
    using ValType = typename iterator_traits<RandomIt>::value_type;
    vector<ValType> sorted(right_end-start_of_range); //len = all elements
    RandomIt sorted_it = sorted.begin();
    //"merge" ranges with vector as buffer
    while (left_it != left_end && right_it != right_end) {
        if(*left_it <= *right_it) {
            //swap the smaller element into buff
            std::swap(*left_it, *sorted_it);
            ++left_it; //advance after inserting
        } else {
            std::swap(*right_it, *sorted_it);
            ++right_it;
        }
        ++sorted_it;
    } //check rest of ranges
    while(left_it != left_end) {
        std::swap(*left_it, *sorted_it);
        ++left_it;
        ++sorted_it;
    }
    while(right_it != right_end) {
        std::swap(*right_it, *sorted_it);
        ++right_it;
        ++sorted_it;
    }
    //now swap the buff vector into original range
    std::swap_ranges(start_of_range, right_end, sorted.begin());
    
    ////compare and swap in-place?
    //    auto mid_it = left_it;
    //    //for that, we need a buffer for the one element
    //    typename iterator_traits<RandomIt>::value_type buff;
    //    while (left_it != left_end && right_it != right_end) {
    //        //choose smallest element out of left, right and buff
    //        if(*left_it > *right_it) {
    //            //swap the left element into buff
    //            std::swap(*right_it, *mid_it);
    //            ++left_it; //advance only l
    //        } else {
    //            //swap right element into buff
    //            std::swap(*left_it, *mid_it);
    //            ++right_it;
    //        }
    //        ++mid_it;
    //    }
    //    //if not finished checking all the elements
    //    if (left_it != left_end || right_it != right_end) {
    //        auto next_element = next(mid_it);
    //        while(next_element != right_end) {
    //            if(*mid_it > *next_element) {
    //                std::swap(*mid_it, *next_element);
    //            }
    //            ++mid_it;
    //            ++next_element;
    //        }
    //    }
    cerr << "===finished joining: ";
    PrintRange(start_of_range, right_end);
    cerr << endl;
}
template <typename RandomIt> //[range_begin, range_end)
void MergeSort(RandomIt start, RandomIt finish) {
    //stack<pair<RandomIt, RandomIt>> sort_stack;
    //empty or one element
    if(start == prev(finish) || start == finish) {
        //cerr << "<-exit recursion with: ";
        //PrintRange(start, finish);
        //cerr << endl;
    } //continue split
    else {
        RandomIt mid =  next(start, (finish-start)/2);
        //cerr << "->launching recursion for: ";
        // PrintRange(start, mid);
        // cerr << " & ";
        // PrintRange(mid, finish);
        // cerr << endl;
        //sort left
        MergeSort(start, mid);
        //sort right half
        MergeSort(mid, finish);
        //"Join"
        MergeRanges(start, mid, finish);
        //std::inplace_merge(start, mid, finish);
    }
}

int main() {
    vector<int> test_vector{9, 4, 8, 1, 6, 7, 2, 10, 5, 3};
    iota(test_vector.begin(), test_vector.end(), 1);
    random_device rd;
    mt19937 g(rd());
    shuffle(test_vector.begin(), test_vector.end(), g);
    
    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());
    cerr << endl;
    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());
    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());
    cerr << endl;
    return 0;
}


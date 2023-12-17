//
//  04.Базовые понятия С++ и STL
//  Тема 11.Тестирование
//  Задача 03.План тестирования
//
//  Created by Pavel Sh on 03.12.2023.
//

/* Тесты!
 Классы эквивалентности: -> (набор данных для тестирования)
 1. Пустая строка
 ""
 2. Строка из пробелов
 "     "
 3. Палиндром/Не палиндром без пробелов (одна буква в середине или 2 одинаковых буквы)
 "a", "abba","radar", "abbc"
 4. Палиндром/Не палиндром из одного слова (пробелы только до и/или после строки)
 "   a ", "    abba ", " radar   ", " asdf   ",
 7. Палиндром/Не палиндром из нескольких слов (пробелы только внутри строки)
 "asd f gfds a", "never odd or even", "aq wees a w tt",
 8. Палиндром/Не палиндром из нескольких слов (+пробелы до/после строки)
 "  dfg hj kjhgf d ", " never odd or even   ", " ffg h h ffd  h "
 
 Output: no, no,yes, yes, yes, no, yes, yes, yes, no, yes, yes, no, yes, yes, no;
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;
//no copy:
bool IsPalindrome(const string& text) {
    if(text.empty()) {
        return false;
    }
    auto fwd_it = text.begin(), bkwd_it = prev(text.end());
    bool letters_present = false;
    while(fwd_it-text.begin() <= bkwd_it-text.begin()) {
        //skip spaces forwards, stop at bkwd_it
        while(*fwd_it == ' ' && fwd_it != bkwd_it) {
            ++fwd_it;
        }//skip spaces backwards, stop at fwd_it
        while(*bkwd_it == ' ' && bkwd_it != fwd_it) {
            --bkwd_it;
        }
        if(*fwd_it != *bkwd_it) {
            return false;
        } else if(*fwd_it != ' ') {
            letters_present = true;
        }
        ++fwd_it;
        --bkwd_it;
    } //return false if no letters other than ' ' were found
    return letters_present;
}

//With copy (for small strings)
/*
bool IsPalindrome(const string& text) {
    //get rid of spaces:
    string text_no_spaces;
    for(auto c : text) {
        if(c != ' ') {
            text_no_spaces.push_back(c);
        }
    }
    if(text_no_spaces.empty()) {
        return false;
    }
    //two center letters equal if size even, one single letter in center if size odd
    for(int head = 0; head < text_no_spaces.size()/2; ++head) {
        int tail = static_cast<int>(text_no_spaces.size()) - 1 - head;
        if(text_no_spaces[head] != text_no_spaces[tail]) {
            return false;
        }
    }
    return true;
}*/
int main() {
//    string str;
//    getline(cin, str);
    
    vector<string> input{"", "       ", "a", "abba","radar", "abbc", "   a ", "    abba ", " radar   ", " asdf   ", "asd f gfds a" , "never odd or even", "aq wees a w tt", "  dfg hj kjhgf d " , " never odd or even   ", " ffg h h ffd  h "};
    for(const auto& str : input) {
        if (IsPalindrome(str)) {
            cout << "palindrome"s << endl;
        } else {
            cout << "not a palindrome"s << endl;
        }
    }
}

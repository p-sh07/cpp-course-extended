//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 07-2.Переводчик
//
//  Created by Pavel Sh on 12.02.2024.
//
#include <cassert>
#include <iostream>
#include <set>
#include <string_view>
#include <unordered_map>

using namespace std;

//v.2: prevent duplicates:
class Translator {
public:
    Translator() {}
    void Add(string_view source, string_view target) {
        string_view src_view = GetCopiedSView(source);
        string_view trgt_view = GetCopiedSView(target);
        //second to last word -> last word
        dict_[src_view] = trgt_view;
        reverse_dict_[trgt_view] = src_view;
    }
    string_view TranslateForward(string_view source) const {
        if(dict_.count(source) == 0) {
            return {};
        }
        return dict_.at(source);
    }
    string_view TranslateBackward(string_view target) const {
        if(reverse_dict_.count(target) == 0) {
            return {};
        }
        return reverse_dict_.at(target);
    }

private:
    string_view GetCopiedSView(string_view word) {
        //if present, returns the string_view, if not present - inserts and returns
        const auto [element, was_inserted] = words_.emplace(word);
        return *element;
    }
    
    set<string> words_;
    unordered_map<string_view, string_view> dict_;
    unordered_map<string_view, string_view> reverse_dict_;
};

//from solution code - cool use of Map* ptr !

//for (const auto* map_ptr : {&forward_dict, &backward_dict}) {
//    const auto it = map_ptr->find(s);
//    if (it != map_ptr->end()) {
//        return it->first;
//    }
//}
//return data.emplace_back(s);

//v.1: assume no duplicate words provided:
//class Translator {
//public:
//    Translator() {}
//    //but, prevent doubles?
//    void Add(string_view source, string_view target) {
//        words_.push_front(string(target));
//        words_.push_front(string(source));
//        //second to last word -> last word
//        dict_[words_[0]] = words_[1];
//        reverse_dict_[words_[1]] = words_[0];
//    }
//    string_view TranslateForward(string_view source) const {
//        if(dict_.count(source) == 0) {
//            return {};
//        }
//        return dict_.at(source);
//    }
//    string_view TranslateBackward(string_view target) const {
//        if(reverse_dict_.count(target) == 0) {
//            return {};
//        }
//        return reverse_dict_.at(target);
//    }
//
//private:
//    deque<string> words_;
//    unordered_map<string_view, string_view> dict_;
//    unordered_map<string_view, string_view> reverse_dict_;
//};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
}

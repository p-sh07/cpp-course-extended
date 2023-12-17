//
//  04.Базовые понятия С++ и STL
//  Тема 16.Итераторы
//  Задача 09.Paginator
//
//  Created by Pavel Sh on 15.12.2023.
//

#include <iostream>
#include <vector>

using std::ostream;
using std::vector;


template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
    : begin_(begin), end_(end)
    {}
    auto begin() const {
        return begin_;
    }
    auto end() const {
        return end_;
    }
    //size
private:
    Iterator begin_, end_;
};

template <typename Iterator>
ostream& operator<<(ostream& out, const IteratorRange<Iterator>& page) {
    for(const auto item : page) {
        out << item;
    }
    return out;
}

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, int page_size) {
        for( ; begin != end; std::advance(begin, page_size) ) {
            if(std::distance(begin, end) < page_size) {
                pages_.push_back({begin, end});
                break;
            }
            pages_.push_back({begin, std::next(begin, page_size)});
        }
    }
    //using It_type = vector<IteratorRange<Iterator>>::iterator;
    auto begin() const {
        return pages_.begin();
    }
    auto end() const {
        return pages_.end();
    }
    //size
private:
    vector<IteratorRange<Iterator>> pages_;
};

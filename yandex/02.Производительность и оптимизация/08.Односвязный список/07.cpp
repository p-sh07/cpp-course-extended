//
//  05.Производительность и оптимизация
//  Тема 8.Односвязный список
//  Урок 07.Работа с массивами
//
//  Created by Pavel Sh on 11.02.2024.
//

#include <cassert>
#include <vector>

template <typename T>
void ReverseArray(T* start, std::size_t size) {
    // Напишите тело функции самостоятельно
    if(size <= 1) {
        return;
    }
    T* tail = start+size-1;
    while (start < tail) {
        T temp = *start;
        
        *start = *tail;
        *tail = temp;
        
        ++start;
        --tail;
    }
}

int main() {
    using namespace std;
    
    vector<int> v = {1, 2, 3, 4, 5};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{5, 4, 3, 2, 1}));
}

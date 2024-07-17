wad//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 07.Совершенствуем парсинг строки
//
//  Created by Pavel Sh on 12.02.2024.
//

#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;


template <typename Container, typename ForwardIt>
ForwardIt LoopIterator(Container& container, ForwardIt pos) {
    return pos == container.end() ? container.begin() : pos;
}

template <typename Container, typename ForwardMoveIt>
ForwardMoveIt MoveLoopIterator(Container& container, ForwardMoveIt pos) {
    return pos == make_move_iterator(container.end()) ? make_move_iterator(container.begin()) : pos;
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    list<typename RandomIt::value_type> pool(make_move_iterator(first), make_move_iterator(last));
    auto cur_pos = pool.begin();
    while (!pool.empty()) {
        *(first++) = std::move(*cur_pos);
        if (pool.size() == 1) {
            break;
        }
        const auto next_pos = LoopIterator(pool, next(cur_pos));
        pool.erase(cur_pos);
        cur_pos = next_pos;
        for (uint32_t step_index = 1; step_index < step_size; ++step_index) {
            cur_pos = LoopIterator(pool, next(cur_pos));
        }
    }
}

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        assert(numbers_copy == numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        assert(numbers_copy == vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем
// об этом далее в нашем курсе

struct NoncopyableInt {
    NoncopyableInt(int v) : value(v) {}
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    assert(numbers == expected);
}

int main() {
    TestIntVector();
    TestAvoidsCopying();
}

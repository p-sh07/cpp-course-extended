//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 09.Std::array<>
//
//  Created by Pavel Sh on 15.02.2024.
//

#include "log_duration.h"
#include "my_assert.h"
#include "stack_vector.h"

#include <iostream>
#include <random>
#include <stdexcept>

using namespace std;

/*
 Метод Capacity должен возвращать вместимость вектора — то есть количество объектов, которое в него может поместиться.
 Метод Size должен возвращать текущее количество объектов в векторе.
 Конструктор принимает размер вектора — по аналогии со стандартным вектором. Если аргумент конструктора больше вместимости вектора, конструктор должен выбрасывать исключение invalid_argument.
 Методы begin/end должны возвращать итераторы на начало и текущий конец вектора. Их тип не указан, выберите его самостоятельно.
 Метод PushBack добавляет новый элемент в конец вектора. Если текущий размер вектора равен его вместимости, метод PushBack должен выбрасывать стандартное исключение overflow_error.
 Метод PopBack уменьшает размер вектора на один и возвращает элемент вектора, который был последним. Если вектор пуст, метод PopBack должен выбрасывать стандартное исключение underflow_error.
 */

void TestConstruction() {
    StackVector<int, 10> v;
    assert(v.Size() == 0u);
    assert(v.Capacity() == 10u);

    StackVector<int, 8> u(5);
    assert(u.Size() == 5u);
    assert(u.Capacity() == 8u);

    try {
        StackVector<int, 10> u(50);
        cout << "Expect invalid_argument for too large size"s << endl;
        assert(false);
    } catch (invalid_argument&) {
    } catch (...) {
        cout << "Expect invalid_argument for too large size"s << endl;
        assert(false);
    }
}

void TestPushBack() {
    StackVector<int, 5> v;
    for (size_t i = 0; i < v.Capacity(); ++i) {
        v.PushBack(i);
    }

    try {
        v.PushBack(0);
        cout << "Expect overflow_error for PushBack in full vector"s << endl;
        assert(false);
    } catch (overflow_error&) {
    } catch (...) {
        cout << "Unexpected exception for PushBack in full vector: "s << endl;
//        assert(false);
    }
}

void TestPopBack() {
    StackVector<int, 5> v;
    for (size_t i = 1; i <= v.Capacity(); ++i) {
        v.PushBack(i);
    }
    for (int i = v.Size(); i > 0; --i) {
        assert(v.PopBack() == i);
    }

    try {
        v.PopBack();
        cout << "Expect underflow_error for PopBack from empty vector"s << endl;
        assert(false);
    } catch (underflow_error&) {
    } catch (...) {
        cout << "Unexpected exception for PopBack from empty vector"s << endl;
        assert(false);
    }
}

void TestOperator() {
    StackVector<int, 5> v;
    for (size_t i = 0; i < v.Capacity(); ++i) {
        v.PushBack(i);
    }
    for (int i = 0; i < v.Size(); ++i) {
        assert(v[i] == i);
    }

    try {
        int x = v[6];
        cout << "Expect out_of_range for [index] >= size() "s << endl;
        assert(false);
    } catch (out_of_range&) {
    } catch (...) {
        cout << "Unexpected exception for out of range index"s << endl;
        assert(false);
    }
}

int main() {
    TestConstruction();
    TestPushBack();
    TestPopBack();
    TestOperator();
    
/*
    cerr << "Running benchmark..."s << endl;
    const size_t max_size = 2500;

    default_random_engine re;
    uniform_int_distribution<int> value_gen(1, max_size);

    vector<vector<int>> test_data(50000);
    for (auto& cur_vec : test_data) {
        cur_vec.resize(value_gen(re));
        for (int& x : cur_vec) {
            x = value_gen(re);
        }
    }

    {
        LOG_DURATION("vector w/o reserve");
        for (auto& cur_vec : test_data) {
            vector<int> v;
            for (int x : cur_vec) {
                v.push_back(x);
            }
        }
    }
    {
        LOG_DURATION("vector with reserve");
        for (auto& cur_vec : test_data) {
            vector<int> v;
            v.reserve(cur_vec.size());
            for (int x : cur_vec) {
                v.push_back(x);
            }
        }
    }
    {
        LOG_DURATION("StackVector");
        for (auto& cur_vec : test_data) {
            StackVector<int, max_size> v;
            for (int x : cur_vec) {
                v.PushBack(x);
            }
        }
    }
    cerr << "Done"s << endl;
 */
}

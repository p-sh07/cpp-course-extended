//
//  05.Производительность и оптимизация
//  Тема 7.Модель памяти
//  Урок 03.Указатели. Открываем доступ к памяти
//
//  Created by Pavel Sh on 20.01.2024.
//
// 3.1 value swap
/*
#include <cassert>
#include <string>

// Шаблонная функция Swap обменивает значения двух объектов одного и того же типа,
// указатели на которые она принимает в качестве аргументов
template <typename T>
void Swap(T* value1, T* value2) {
    // Реализуйте тело функции самостоятельно
    T temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

int main() {
    using namespace std;

    {
        int x = 1;
        int y = 5;

        Swap(&x, &y);
        assert(x == 5);
        assert(y == 1);
    }

    {
        string one = "one"s;
        string two = "two"s;
        Swap(&one, &two);
        assert(one == "two"s);
        assert(two == "one"s);
    }

    {
        // Видимость структуры Point ограничена текущим блоком
        struct Point {
            int x, y;
        };

        Point p1{1, 2};
        Point p2{3, 4};

        Swap(&p1, &p2);
        assert(p1.x == 3 && p1.y == 4);
        assert(p2.x == 1 && p2.y == 2);
    }
    return 0;
}as

int main() {
    using namespace std;

    {
        int x = 1;
        int y = 5;

        Swap(&x, &y);
        assert(x == 5);
        assert(y == 1);
    }

    {
        string one = "one"s;
        string two = "two"s;
        Swap(&one, &two);
        assert(one == "two"s);
        assert(two == "one"s);
    }

    {
        // Видимость структуры Point ограничена текущим блоком
        struct Point {
            int x, y;
        };

        Point p1{1, 2};
        Point p2{3, 4};

        Swap(&p1, &p2);
        assert(p1.x == 3 && p1.y == 4);
        assert(p2.x == 1 && p2.y == 2);
    }
    return 0;
}
*/

#include <cassert>
#include <string>

//template </* Напишите недостающий код самостоятельно */>
//bool IsSameObject(/*Напишите недостающий код самостоятельно*/) {
//    /*Напишите тело функции самостоятельно */
//}

template <typename T>
bool IsSameObject(T& first, T& second) {
    return &first == &second;
}

int main() {
    using namespace std;

    const int x = 1;
    const int y = 1;
    // x и y — разные объекты, хоть и имеющие одинаковое значение
    assert(!IsSameObject(x, y));
    // Оба аргумента — один и тот же объект
    assert(IsSameObject(x, x));

    const string name1 = "Harry"s;
    const string name1_copy = name1;
    const string name2 = "Ronald"s;
    auto name1_ptr = &name1;
    const string& name1_ref = name1;

    assert(!IsSameObject(name1, name2));  // Две строки с разными значениями — разные объекты
    assert(!IsSameObject(name1, name1_copy));  // Строка и её копия — разные объекты

    // Оба параметра ссылаются на одну и ту же строку
    assert(IsSameObject(name1, name1));
    assert(IsSameObject(name2, name2));
    assert(IsSameObject(name1_copy, name1_copy));

    // Разыменованный указатель на объект и сам объект — один и тот же объект
    assert(IsSameObject(*name1_ptr, name1));

    // Переменная и ссылка на неё относятся к одному и тому же объекту
    assert(IsSameObject(name1_ref, name1));
    // Ссылка на объект и разыменованный указатель на объект относятся к одному и тому же объекту
    assert(IsSameObject(name1_ref, *name1_ptr));
}

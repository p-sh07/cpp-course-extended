//
//  main.cpp
//  cpp-practice
//
//  Created by ps on 18.04.2024.
//

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <string>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init)
    : init_(init) {
    }

    bool HasValue() const {
        return obj_.has_value();
    }
    const T& Get() const {
        if(!HasValue()) {
            obj_ = init_();
        }
        return obj_.value();
    }

private:
    function<T()> init_;
    mutable optional<T> obj_;
};

void UseExample() {
    string big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
    });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}
/*
Используйте сигнатуры и названия из заготовки кода.
У класса должен быть один шаблонный параметр T — тип элемента, указатель на который будет храниться внутри.
Напишите следующие функции в классе:
Конструктор по умолчанию, создающий пустой умный указатель.
Конструктор, принимающий T* и захватывающий владение этой динамической памятью.
Конструктор перемещения, получающий на вход rvalue-ссылку на другой UniquePtr и отбирающий у него владение ресурсом.
Оператор присваивания, получающий на вход nullptr (тип - nullptr_t, определенный в заголовочном файле cstddef). В результате умный указатель должен стать пустым.
Move-оператор присваивания, получающий на вход rvalue-ссылку на другой UniquePtr.
Деструктор.
Оператор *.
Оператор -> — он должен вернуть указатель на обычную структуру, к которой можно применить обычный ->.
Функцию T* Release() — она отменяет владение объектом и возвращает хранящийся внутри указатель.
Функцию void Reset(T* ptr), после выполнения которой умный указатель должен захватить ptr.
Функцию void Swap(UniquePtr& other), обменивающуюся содержимым с другим умным указателем.
Функцию T* Get() const, возвращающую указатель.
Ограничения
В вашем классе должны быть запрещены конструктор копирования и обычный оператор присваивания.
Использовать стандартный unique_ptr и заголовочный файл memory нельзя.
В этой задаче не нужно делать специализацию для массива и не нужно предусматривать свой Deleter.
*/

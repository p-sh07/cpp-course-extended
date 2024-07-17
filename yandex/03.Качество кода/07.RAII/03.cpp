//
//  03.Качество кода
//  Тема
//  Урок 
//
//  Created by ps on 20.04.2024.
//

#include <cassert>
#include <cstddef>  // нужно для nullptr_t
#include <utility>

using namespace std;

/*
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

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* data_ptr_ = nullptr;
    
public:
    UniquePtr()
    : data_ptr_(nullptr) {
    }
    explicit UniquePtr(T* ptr)
    :data_ptr_(ptr) {
    }
    explicit UniquePtr(UniquePtr&& other)
    :data_ptr_(other.Release()) {
    }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    UniquePtr& operator=(nullptr_t) {
        delete data_ptr_;
        data_ptr_ = nullptr;
        return *this;
    }
    
    UniquePtr& operator=(UniquePtr&& other) {
        if(&other != this) {
            Swap(other);
        }
        other = nullptr;
        return *this;
    }
    
    ~UniquePtr() {
        delete data_ptr_;
    }

    T& operator*() const {
        return *data_ptr_;
    }
    T* operator->() const {
        return data_ptr_;
    }
    T* Release() {
        auto temp = data_ptr_;
        data_ptr_= nullptr;
        return temp;
    }
    void Reset(T* ptr) {
        delete data_ptr_;
        data_ptr_ = ptr;
        
    }
    void Swap(UniquePtr& other) {
        std::swap(data_ptr_, other.data_ptr_);
    }
    T* Get() const {
        return data_ptr_;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);
        
        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}

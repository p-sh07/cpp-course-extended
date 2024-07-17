//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 11.Simple vector
//
//  Created by Pavel Sh on 15.02.2024.
//

#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include "array_ptr.h"

// Класс-обертка для вызова резервирующего конмтруктора
class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t val)
    : val_(val) {}
    
    size_t GetValue() const {
        return val_;
    }
private:
    size_t val_ = 0;
};

// Функция для вызова необходимого конструктора
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};


template <typename Type>
class SimpleVector {
public:
    //=================== Конструкторы =================== //
    SimpleVector() noexcept = default;
    
    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);
    
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value);
    
    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);
    
    // Создаёт вектор из std::initializer_list - move
    SimpleVector(std::initializer_list<Type>&& init);
    
    // Резервирующий конструктор
    SimpleVector(const ReserveProxyObj& reserve_obj);
    
    // Конструктор копирования
    SimpleVector(const SimpleVector& other);
    
    // Конструктор перемещения
    SimpleVector(SimpleVector&& other);
    
    //=================== Операторы =================== //
    // Оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs);
    
    // Оператор перемещения
    SimpleVector& operator=(SimpleVector&& rhs);
    
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept;
    
    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept;
    
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    //=================== Публичные методы =================== //
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept;
    
    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept;
    
    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept;
    
    // Добавляет элемент в конец вектора
    void PushBack(const Type& item);
    
    // Добавляет элемент в конец вектора
    void PushBack(Type&& item);
    
    // Вставляет значение value в позицию pos.
    Iterator Insert(ConstIterator pos, const Type& value);
    
    // Вставляет значение value в позицию pos.
    Iterator Insert(ConstIterator pos, Type&& value);
    
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;
    
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);
    
    // Меняет поля this c other
    void swap(SimpleVector& other) noexcept;
    
    // Возвращает константную ссылку на элемент с индексом index
    Type& At(size_t index);
    
    // Возвращает константную ссылку на элемент с индексом index
    const Type& At(size_t index) const;
    
    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept;
    
    // Изменяет размер массива.
    void Resize(size_t new_size);
    
    // Задает ёмкость вектора
    void Reserve(size_t new_capacity);

    //=================== Итераторы =================== //
    // Возвращает итератор на начало массива
    Iterator begin() noexcept;
    
    // Возвращает итератор на элемент, следующий за последним
    Iterator end() noexcept;
    
    // Возвращает константный итератор на начало массива
    ConstIterator begin() const noexcept;
    
    // Возвращает итератор на элемент, следующий за последним
    ConstIterator end() const noexcept;
    
    // Возвращает константный итератор на начало массива
    ConstIterator cbegin() const noexcept;
    
    // Возвращает итератор на элемент, следующий за последним
    ConstIterator cend() const noexcept;
    
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> data_;
    
    void ReallocateMemory(size_t new_capacity = 0, bool reserve = false);
    void FillWithDefaultVal(Iterator first, Iterator last);

};


//=================== Конструкторы =================== //
// Создаёт вектор из size элементов, инициализированных значением по умолчанию
template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size)
: size_(size), capacity_(size)
, data_(ArrayPtr<Type>(size_)) {
    FillWithDefaultVal(begin(), end());
}

// Создаёт вектор из size элементов, инициализированных значением value
template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type& value)
: size_(size), capacity_(size)
, data_(ArrayPtr<Type>(size_)) {
    std::fill(begin(), end(), value);
}

// Создаёт вектор из std::initializer_list
template<typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> init)
: size_(init.size()), capacity_(init.size())
, data_(ArrayPtr<Type>(size_)){
    std::copy(init.begin(), init.end(), begin());
}

// Создаёт вектор из std::initializer_list - move версия
template<typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type>&& init)
: size_(init.size()), capacity_(init.size())
, data_(ArrayPtr<Type>(size_)){
    std::move(init.begin(), init.end(), begin());
}

// Резервирующий конструктор
template<typename Type>
SimpleVector<Type>::SimpleVector(const ReserveProxyObj& reserve_obj)
: size_(0), capacity_(reserve_obj.GetValue())
, data_(ArrayPtr<Type>(capacity_)) {}

// Конструктор копирования
template<typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other)
: size_(other.size_), capacity_(other.size_)
, data_(ArrayPtr<Type>(other.size_)){
    std::copy(other.begin(), other.end(), begin());
}

// Конструктор перемещения
template<typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector&& other) {
    data_ = std::move(other.data_); //other.data_ is deleted here
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
}


//=================== Операторы =================== //
// Возвращает ссылку на элемент с индексом index
template<typename Type>
Type& SimpleVector<Type>::operator[](size_t index) noexcept {
    assert(index < size_);
    return data_[index];
}

// Возвращает константную ссылку на элемент с индексом index
template<typename Type>
const Type& SimpleVector<Type>::operator[](size_t index) const noexcept {
    assert(index < size_);
    return data_[index];
}

// Оператор копирования
template<typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector& rhs) {
    if(this != &rhs) {
        SimpleVector<Type> temp(rhs);
        swap(temp);
    }
    return *this;
}

// Оператор перемещения
template<typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(SimpleVector&& rhs) {
    if(this != &rhs) {
        //move calls operator=(&&) of ArrayPtr, which deletes[] rhs.raw_ptr_
        data_ = std::move(rhs.data_);
        size_ = std::exchange(rhs.size_, 0);
        capacity_ = std::exchange(rhs.capacity_, 0);
    }
    return *this;
}


//=================== Публичные методы =================== //
// Возвращает количество элементов в массиве
template<typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept {
    return size_;
}

// Возвращает вместимость массива
template<typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept {
    return capacity_;
}

// Сообщает, пустой ли массив
template<typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept {
    return (size_ == 0);
}

// Возвращает константную ссылку на элемент с индексом index
// Выбрасывает исключение std::out_of_range, если index >= size
template<typename Type>
Type& SimpleVector<Type>::At(size_t index) {
    if(index >= size_) {
        throw std::out_of_range("Index is out of SimpleVector range");
    }
    return data_[index];
}

// Возвращает константную ссылку на элемент с индексом index
// Выбрасывает исключение std::out_of_range, если index >= size
template<typename Type>
const Type& SimpleVector<Type>::At(size_t index) const {
    if(index >= size_) {
        throw std::out_of_range("Index is out of SimpleVector range");
    }
    return data_[index];
}

// Обнуляет размер массива, не изменяя его вместимость
template<typename Type>
void SimpleVector<Type>::Clear() noexcept {
    size_ = 0;
}

// Изменяет размер массива.
// При увеличении размера новые элементы получают значение по умолчанию для типа Type
template<typename Type>
void SimpleVector<Type>::Resize(size_t new_size) {
    if(new_size > size_) {
        if(new_size < capacity_) {
            //within cap, no need to reallocate, fill additional items with default val
            FillWithDefaultVal(begin()+size_, begin()+new_size);
        } else {
            ReallocateMemory(new_size);
        }
    }
    //update size_ in every case
    size_ = new_size;
}


// Добавляет элемент в конец вектора
// При нехватке места увеличивает вдвое вместимость вектора
template<typename Type>
void SimpleVector<Type>::PushBack(const Type& item) {
    Resize(size_ + 1);
    *std::prev(end()) = item;
}

// Добавляет элемент в конец вектора - move
template<typename Type>
void SimpleVector<Type>::PushBack(Type&& item) {
    Resize(size_ + 1);
    *std::prev(end()) = std::exchange(item, Type{});
}

// Вставляет значение value в позицию pos.
// Возвращает итератор на вставленное значение
// Если перед вставкой значения вектор был заполнен полностью,
// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, const Type& value) {
    //store the position index before possible reallocation
    size_t pos_index = pos - cbegin();
    Resize(size_ + 1);
    Iterator insert_pos = begin() + pos_index;
    std::move_backward(insert_pos, end()-1, end());
    *insert_pos = value;
    return insert_pos;
}

// Вставляет значение value в позицию pos - move
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, Type&& value) {
    //store the position index before possible reallocation
    size_t pos_index = pos - cbegin();
    Resize(size_ + 1);
    Iterator insert_pos = begin() + pos_index;
    std::move_backward(insert_pos, end()-1, end());
    *insert_pos = std::exchange(value, Type{});
    return insert_pos;
}

// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
template<typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    assert(!IsEmpty());
    --size_;
}

// Удаляет элемент вектора в указанной позиции
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>:: Erase(ConstIterator pos) {
    assert(!IsEmpty());
    Iterator erase_pos = begin() + (pos - cbegin());
    std::move(erase_pos + 1, end(), erase_pos);
    --size_;
    return erase_pos;
}

// Обменивает значение с другим вектором
template<typename Type>
void SimpleVector<Type>::swap(SimpleVector& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    data_.swap(other.data_);
}

// Резервирует память
template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if(capacity_ < new_capacity) {
        ReallocateMemory(new_capacity, true);
    }
}


//=================== Итераторы =================== //
// Возвращает итератор на начало массива
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept {
    return data_.Get();
}

// Возвращает итератор на элемент, следующий за последним
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept {
    return data_.Get() + size_;
}

// Возвращает константный итератор на начало массива
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept {
    return data_.Get();
}

// Возвращает итератор на элемент, следующий за последним
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept {
    return data_.Get() + size_;
}

// Возвращает константный итератор на начало массива
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept {
    return data_.Get();
}

// Возвращает итератор на элемент, следующий за последним
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept {
    return data_.Get() + size_;
}


//=================== Приватные методы =================== //
// Выделение нового массива в памяти с 2x вместимостью,
// либо указанного размера, если он больше 2*capacity_
template<typename Type>
void SimpleVector<Type>::ReallocateMemory(size_t new_capacity, bool reserve) {
    if(!reserve) {
        new_capacity = std::max(new_capacity, 2 * capacity_);
    }

    ArrayPtr<Type> temp(new_capacity);
    std::move(begin(), end(), temp.Get());
    
    //fill the rest of new array with Type{} default values
    FillWithDefaultVal(temp.Get() + size_, temp.Get() + new_capacity);
    
    data_.swap(temp);
    capacity_ = new_capacity;
}

// Заполнить интервал значением Type по умолчанию, избегает копирования
template<typename Type>
void SimpleVector<Type>::FillWithDefaultVal(Iterator first, Iterator last) {
    for( ;first != last; ++first) {
        *first = Type{};
    }
}

/* for debug
 template<typename Type>
 void SimpleVector<Type>::Print() {
     cerr << "[sz = " << size_ << "][cp = " << capacity_ << "]{";
     bool is_first = true;
     for(size_t i = 0; i < size_; ++i) {
         if(!is_first) {
             cerr << ", ";
         }
         is_first = false;
         cerr << data_[i];
     }
     cerr << "}" << endl;
 }
 */

//====== ***End of SimpleVector class Definitions*** ======//


//=================== Перегрузки операторов =================== //
template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && !(lhs < rhs) && !(rhs < lhs);
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}


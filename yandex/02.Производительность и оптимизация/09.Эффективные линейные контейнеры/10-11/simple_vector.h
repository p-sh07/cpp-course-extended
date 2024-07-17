#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include "array_ptr.h"

//Part3: Reserve constructor!
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

//Part1: SimpleVector
template <typename Type>
class SimpleVector {
public:
    SimpleVector() noexcept = default;
    
    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value);
    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);
    // Конструктор копирования
    SimpleVector(const SimpleVector& other);
    // Резервирующий конструктор
    SimpleVector(const ReserveProxyObj& reserve_obj);
    
    // Оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs);
    
    using Iterator = Type*;
    using ConstIterator = const Type*;
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept;
    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept;
    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept;
    // Добавляет элемент в конец вектора
    void PushBack(const Type& item);
    // Вставляет значение value в позицию pos.
    Iterator Insert(ConstIterator pos, const Type& value);
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;
    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);
    //
    void swap(SimpleVector& other) noexcept;
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept;
    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept;
    
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

};

//Part 1:
// Создаёт вектор из size элементов, инициализированных значением по умолчанию
template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size)
: size_(size), capacity_(size)
, data_(ArrayPtr<Type>(size_)) {
    std::fill(begin(), end(), Type{});
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

// Резервирующий конструктор
template<typename Type>
SimpleVector<Type>::SimpleVector(const ReserveProxyObj& reserve_obj)
: size_(0), capacity_(reserve_obj.GetValue())
, data_(ArrayPtr<Type>(capacity_)) {}

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

// Возвращает ссылку на элемент с индексом index
template<typename Type>
Type& SimpleVector<Type>::operator[](size_t index) noexcept {
    return data_[index];
}

// Возвращает константную ссылку на элемент с индексом index
template<typename Type>
const Type& SimpleVector<Type>::operator[](size_t index) const noexcept {
    return data_[index];
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
            std::fill(begin()+size_, begin()+new_size, Type{});
        } else {
            ReallocateMemory(new_size);
        }
    }
    //update size_ in every case
    size_ = new_size;
}

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
    return data_.Get()+size_;
}

// Возвращает константный итератор на начало массива
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept {
    return &data_[0];
}

// Возвращает итератор на элемент, следующий за последним
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept {
    return &data_[size_];
}

// Возвращает константный итератор на начало массива
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept {
    return &data_[0];
}

// Возвращает итератор на элемент, следующий за последним
// Для пустого массива может быть равен (или не равен) nullptr
template<typename Type>
SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept {
    return &data_[size_];
}


//Part 2
template<typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other) 
: size_(other.size_), capacity_(other.size_)
, data_(ArrayPtr<Type>(other.size_)){
    std::copy(other.begin(), other.end(), begin());
}

template<typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector& rhs) {
    SimpleVector<Type> temp(rhs);
    swap(temp);
    return *this;
}

// Добавляет элемент в конец вектора
// При нехватке места увеличивает вдвое вместимость вектора
template<typename Type>
void SimpleVector<Type>::PushBack(const Type& item) {
    Resize(size_+1);
    *std::prev(end()) = item;
}

// Вставляет значение value в позицию pos.
// Возвращает итератор на вставленное значение
// Если перед вставкой значения вектор был заполнен полностью,
// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
template<typename Type>
SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, const Type& value) {
    //store the position index before possible reallocation
    size_t pos_index = pos - cbegin();
    Resize(size_+1);
    Iterator insert_pos = begin() + pos_index;
    std::copy_backward(insert_pos, end()-1, end());
    *insert_pos = value;
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
    Iterator erase_pos = begin() + (pos - cbegin());
    std::copy(erase_pos+1, end(), erase_pos);
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
// Выделение нового массива в памяти с 2x вместимостью,
// либо указанного размера, если он больше 2*capacity_
template<typename Type>
void SimpleVector<Type>::ReallocateMemory(size_t new_capacity, bool reserve) {
    if(!reserve) {
        new_capacity = std::max(new_capacity, 2*capacity_);
    }

    ArrayPtr<Type> temp(new_capacity);
    std::copy(begin(), end(), temp.Get());
    
    //fill the rest of new array with Type{} default values
    std::fill(temp.Get()+size_, temp.Get()+new_capacity, Type{});
    
    data_.swap(temp);
    capacity_ = new_capacity;
}

template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if(capacity_ < new_capacity) {
        ReallocateMemory(new_capacity, true);
    }
}
//===================================//

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

/*
 void Print
 {
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


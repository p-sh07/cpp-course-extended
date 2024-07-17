//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 09.Std::array<>
//
//  Created by Pavel Sh on 15.02.2024.
//


#pragma once

#include <array>
#include <stdexcept>

using std::array;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if(N < a_size) {
            throw std::invalid_argument("Size exceeds capacity");
        }
        size_ = a_size;
    }
    
    T& operator[](size_t index) {
//        if(size_ <= index) {
//            throw std::out_of_range("Index it out of range");
//        }
        return data_[index];
    }
    const T& operator[](size_t index) const {
//        if(size_ <= index) {
//            throw std::out_of_range("Index it out of range");
//        }
        return data_[index];
    }
    
    using value_type = T;
    using iterator = array<T, N>::iterator;
    using const_iterator = array<T, N>::const_iterator;
    
    iterator begin() {
        return data_.begin();
    }
    iterator end() {
        return data_.begin() + size_;
    }
    const_iterator begin() const {
        return data_.cbegin();
    }
    const_iterator end() const {
        return data_.cbegin() + size_;
    }
    
    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return N;
    }
    
    void PushBack(const T& value) {
        if(size_ == N) {
            throw std::overflow_error("Max capacity reached");
        }
        data_[size_++] = value;
    }
    T PopBack() {
        if(size_ == 0) {
            throw std::underflow_error("data_ is empty");
        }
        return data_[--size_];
    }
private:
    size_t size_ = 0;
    array<T,N> data_;
};

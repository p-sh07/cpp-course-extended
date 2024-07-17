#pragma once
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;
    
    explicit RawMemory(size_t capacity)
    : buffer_(Allocate(capacity))
    , capacity_(capacity) {
    }
    
    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory& rhs) = delete;
    
    RawMemory(RawMemory&& other) noexcept
    : buffer_(other.buffer_)
    , capacity_(other.capacity_) {
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }
    
    RawMemory& operator=(RawMemory&& rhs) noexcept {
        Swap(rhs);
        return *this;
    }
    
    
    ~RawMemory() {
        Deallocate(buffer_);
    }
    
    T* operator+(size_t offset) noexcept {
        // Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
        assert(offset <= capacity_);
        return buffer_ + offset;
    }
    
    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }
    
    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }
    
    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }
    
    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }
    
    const T* GetAddress() const noexcept {
        return buffer_;
    }
    
    T* GetAddress() noexcept {
        return buffer_;
    }
    
    T* GetAddress(size_t index) noexcept {
        return buffer_+index;
    }
    
    size_t Capacity() const {
        return capacity_;
    }
    
private:
    // Выделяет сырую память под n элементов и возвращает указатель на неё
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }
    
    // Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }
    
    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
    Vector() = default;
    
    explicit Vector(size_t size)
    : data_(size)
    , size_(size) {
        std::uninitialized_value_construct_n(data_.GetAddress(), size);
    }
    
    Vector(const Vector& other)
    : data_(other.size_)
    , size_(other.size_)
    
    {
        std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
    }
    
    Vector(Vector&& other) noexcept {
        data_ = std::move(other.data_);
        size_ = other.size_;
    }
    /*
     Создайте переменную size_t copy_count = std::min(size_, rhs.size_);. С помощью copy_n скопируйте copycount элементов. Если `size < rhs.size, то нужно докопировать элементы. Если size > rhs.size_`, то нужно удалить лишние элементы
     */
    Vector& operator=(const Vector& rhs)    {
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                //create a new vec and copy other vector's contents there, then swap
                Vector<T> new_vec(rhs);
                Swap(new_vec);
            } else {
                size_t copy_count = std::min(size_, rhs.size_);
                
                //std::copy_n(rhs.data_.GetAddress(), copy_count, data_.GetAddress());
                for(size_t cp = 0; cp < copy_count; ++cp) {
                    data_[cp] = rhs.data_[cp];
                }
                
                
                if(rhs.size_ < size_) {
                    //удалить лишние элементы
                    for (size_t i_delete = rhs.size_; i_delete < size_; ++ i_delete) {
                        Destroy(data_.GetAddress() + i_delete);
                    }
                } else {
                    //докопировать оставшиеся
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + copy_count, rhs.size_-copy_count, data_.GetAddress()+ copy_count);
                }
                size_ = rhs.size_;
            }
        }
        return *this;
    }
    
    Vector& operator=(Vector&& rhs)    {
        if(this != &rhs) {
            Swap(rhs);
        }
        return *this;
    }
    
    void Swap(Vector& other) noexcept {
        if(this != &other) {
            data_.Swap(other.data_);
            std::swap(size_, other.size_);
        }
    }
    
    ~Vector() {
        if(data_.Capacity() > 0) {
            std::destroy_n(data_.GetAddress(), size_);
        }
    }
    
    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_memory(new_capacity);
        MoveOrCopyAllItems(new_memory);
    }
    
    void Resize(size_t new_size) {
        if(size_ < new_size) {
            Reserve(new_size);
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
        }
        else {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
        }
        size_ = new_size;
    }
    
    template<typename S> void PushBack(S&& value) { //->uses forwarding reference for const T& and T&&
        //no more space for an extra element
        if(Capacity() == size_ || Capacity() == 0) {
            //Allocate new mem and copy the element there
            RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
            new(new_data.GetAddress()+size_) T(std::forward<S>(value));
            
            MoveOrCopyAllItems(new_data);
        }
        else {
            new(data_.GetAddress()+size_) T(std::forward<S>(value));
        }
        ++size_;
    }
    
    void PopBack() noexcept {
        Destroy(data_.GetAddress() + size_ - 1);
        --size_;
    }
    
    size_t Size() const noexcept {
        return size_;
    }
    
    size_t Capacity() const noexcept {
        return data_.Capacity();
    }
    
    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }
    
    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }
    
private:
    RawMemory<T> data_;
    size_t size_ = 0;
    
    //make sure new memory capacity is enough!
    void MoveOrCopyAllItems(RawMemory<T>& copy_to_and_swap) {
        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, copy_to_and_swap.GetAddress());
        } else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, copy_to_and_swap.GetAddress());
        }
        data_.Swap(copy_to_and_swap);
        std::destroy_n(copy_to_and_swap.GetAddress(), size_);
    }
    
    // Вызывает деструктор объекта по адресу buf
    static void Destroy(T* buf) noexcept {
        buf->~T();
    }
};

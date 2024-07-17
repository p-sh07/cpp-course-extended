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
    
    const T* GetAddress(size_t index) const noexcept {
        return buffer_+index;
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
    , size_(other.size_) {
        std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
    }
    
    Vector(Vector&& other) noexcept {
        data_ = std::move(other.data_);
        size_ = other.size_;
    }
    
    ~Vector() {
        if(data_.Capacity() > 0) {
            std::destroy_n(data_.GetAddress(), size_);
        }
    }
    
    Vector& operator=(const Vector& rhs)    {
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                //create a new vec and copy other vector's contents there, then swap
                Vector<T> new_vec(rhs);
                Swap(new_vec);
            } else {
                size_t copy_count = std::min(size_, rhs.size_);
                
                std::copy_n(rhs.data_.GetAddress(), copy_count, data_.GetAddress());
                
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
    
    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }
    
    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }
    
    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_memory(new_capacity);
        MoveOrCopyItems(new_memory, 0, size_);
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
    
    size_t Size() const noexcept {
        return size_;
    }
    
    size_t Capacity() const noexcept {
        return data_.Capacity();
    }
    
    const T& Back() const {
        return *std::prev(end());
    }
    
    T& Back() {
        return *std::prev(end());
    }
    
    using iterator = T*;
    using const_iterator = const T*;
    
    iterator begin() noexcept {
        return data_.GetAddress();
    }
    iterator end() noexcept {
        return data_.GetAddress(size_);
    }
    const_iterator begin() const noexcept {
        return data_.GetAddress();
    }
    const_iterator end() const noexcept {
        return data_.GetAddress(size_);
    }
    const_iterator cbegin() const noexcept {
        return data_.GetAddress();
    }
    const_iterator cend() const noexcept {
        return data_.GetAddress(size_);
    }
    
    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        auto pos_it = const_cast<iterator>(pos);
        
        return (Capacity() == size_ || Capacity() == 0)
        ? ReallocateEmplace(pos_it, std::forward<Args>(args)...)
        : ShiftEmplace(pos_it, std::forward<Args>(args)...);
    }
    
    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }
    
    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }
    
    iterator Erase(const_iterator pos){
        //Destroy element at pos and left-move other items
        iterator erase_pos = const_cast<iterator>(pos);
        Destroy(erase_pos);
        std::move(erase_pos + 1, end(), erase_pos);
        --size_;
        return erase_pos;
    }
    
    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        return *Emplace(end(), std::forward<Args>(args)...);
    }
    
    void PushBack(const T& value) {
        EmplaceBack(value);
    }
    
    void PushBack(T&& value) {
        EmplaceBack(std::move(value));
    }
    
    void PopBack() noexcept {
        Erase(end()-1);
    }
    
    void Swap(Vector& other) noexcept {
        if(this != &other) {
            data_.Swap(other.data_);
            std::swap(size_, other.size_);
        }
    }
private:
    RawMemory<T> data_;
    size_t size_ = 0;
    
    //make sure new memory capacity is enough!
    void MoveOrCopyItems(RawMemory<T>& copy_to_and_swap, size_t start_pos, size_t num_items, bool swap = true, bool shift_right = false) {

        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress()+start_pos, num_items,
                                      copy_to_and_swap.GetAddress() + start_pos + (shift_right ? 1 : 0));
        } else {
            std::uninitialized_copy_n(data_.GetAddress()+start_pos, num_items,
                                      copy_to_and_swap.GetAddress() + start_pos + (shift_right ? 1 : 0));
        }
        if(swap) {
            data_.Swap(copy_to_and_swap);
            std::destroy_n(copy_to_and_swap.GetAddress(), size_);
        }
    }
    
    template <typename... Args>
    iterator ReallocateEmplace(iterator pos, Args&&... args) {
        size_t pos_index = pos - begin();
        //Allocate new mem and forward the element there
        RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
        new(&new_data[pos_index]) T(std::forward<Args>(args)...);
        
        //Move items from begin -> insert pos(excluding), to new mem:
        MoveOrCopyItems(new_data, 0, pos_index, false, false);
        
        //Shift items after pos from old mem to new mem with a right-shift, swap & destroy old mem
        MoveOrCopyItems(new_data, pos_index, size_ - pos_index, true, true);
        
        ++size_;
        return begin() + pos_index;
    }
    
    template <typename... Args>
    iterator ShiftEmplace(iterator pos, Args&&... args) {
        if(size_ > 0 && pos != end()) {
            T temp(std::forward<Args>(args)...);
            //allocate possibly unallocated memory after end() - ?
            new(end()) T(std::move(data_[size_-1]));
            
            std::move_backward(pos, end()-1, end());
            *pos = std::move(temp);
        } else {
            //emplace new item
            new(pos) T(std::forward<Args>(args)...);
        }
        ++size_;
        return pos;
    }
    
    // Вызывает деструктор объекта по адресу buf
    static void Destroy(T* buf) noexcept {
        buf->~T();
    }
};

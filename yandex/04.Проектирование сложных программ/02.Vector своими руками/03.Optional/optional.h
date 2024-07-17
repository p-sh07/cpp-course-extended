#include <stdexcept>
#include <utility>
#include <memory>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value) {
        val_ptr_ = new (&data_[0]) T(value);
        is_initialized_ = true;
    }
    Optional(T&& value) {
        val_ptr_ = new (&data_[0]) T(std::move(value));
        is_initialized_ = true;
    }
    Optional(const Optional& other) {
        if(other.HasValue()) {
            val_ptr_ = new (&data_[0]) T(other.Value());
            is_initialized_ = true;
        } else {
            
        }
    }
    Optional(Optional&& other) {
        if(other.HasValue()) {
            val_ptr_ = new (&data_[0]) T(std::move(other.Value()));
//            other.Reset();
            is_initialized_ = true;
        } else {
            //??
        }
    }

    Optional& operator=(const T& value) {
        if(is_initialized_) {
            *val_ptr_ = value;
        } else {
            val_ptr_ = new (&data_[0]) T(value);
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(T&& rhs) {
        if(is_initialized_) {
            *val_ptr_ = std::move(rhs);
        } else {
            val_ptr_ = new (&data_[0]) T(std::move(rhs));
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(const Optional& rhs) {
        if(rhs.HasValue()) {
            *this = rhs.Value();
        } else {
            if(HasValue()) {
                Reset();
            } else {
                *this = T{};
            }
        }
        return *this;
    }
    Optional& operator=(Optional&& rhs) {
        if(rhs.HasValue()) {
            *this = std::move(rhs.Value());
        } else {
            if(HasValue()) {
                Reset();
            } else {
                *this = T{};
            }
        }
        return *this;
    }

    ~Optional() {
        if(HasValue()) {
            val_ptr_->~T();
            val_ptr_ = nullptr;
        };
    }
    bool HasValue() const {
        return is_initialized_ && val_ptr_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() {
        return *val_ptr_;
    }
    const T& operator*() const {
        return *val_ptr_;
    }
    T* operator->() {
        return val_ptr_;
    }
    const T* operator->() const {
        return val_ptr_;
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
        if(!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *val_ptr_;
    }
    const T& Value() const {
        return *val_ptr_;
    }

    void Reset() {
        if(is_initialized_ && val_ptr_) {
            val_ptr_->~T();
            val_ptr_ = nullptr;
        }
    }

    T& operator*() {
        return *val_ptr_;
    }
 
    T* operator->() {
        return val_ptr_;
    }
    
private:
    T* val_ptr_ = nullptr;
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};

//Author solution:
/*
#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;

    Optional(const T& value) {
        new (data_) T(value);
        is_initialized_ = true;
    }

    Optional(T&& value) {
        new (data_) T(std::move(value));
        is_initialized_ = true;
    }

    Optional(const Optional& other) {
        if (other.is_initialized_) {
            new (data_) T(*other);
            is_initialized_ = true;
        }
    }

    Optional(Optional&& other) {
        if (other.is_initialized_) {
            new (data_) T(std::move(*other));
            is_initialized_ = true;
        }
    }

    Optional& operator=(const T& value) {
        if (is_initialized_) {
            **this = value;
        } else {
            new (data_) T(value);
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(T&& rhs) {
        if (is_initialized_) {
            **this = std::move(rhs);
        } else {
            new (data_) T(std::move(rhs));
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(const Optional& rhs) {
        if (this != &rhs) {
            if (!rhs.is_initialized_) {
                Reset();
            } else if (is_initialized_) {
                **this = *rhs;
            } else {
                new (data_) T(*rhs);
                is_initialized_ = true;
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& rhs) {
        if (!rhs.is_initialized_) {
            Reset();
        } else if (is_initialized_) {
            **this = std::move(*rhs);
        } else {
            new (data_) T(std::move(*rhs));
            is_initialized_ = true;
        }
        return *this;
    }

    ~Optional() {
        Reset();
    }

    bool HasValue() const {
        return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() {
        return *reinterpret_cast<T*>(&data_[0]);
    }

    const T& operator*() const {
        return *reinterpret_cast<const T*>(&data_[0]);
    }

    T* operator->() {
        return reinterpret_cast<T*>(data_);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(data_);
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return **this;
    }

    const T& Value() const {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return **this;
    }

    void Reset() {
        if (is_initialized_) {
            Value().~T();
            is_initialized_ = false;
        }
    }

private:
    // alignas должен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};
*/

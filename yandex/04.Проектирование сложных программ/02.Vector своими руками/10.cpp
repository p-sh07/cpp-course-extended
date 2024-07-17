#include <cassert>
#include <memory>
#include <string>
#include <string_view>

//Part 2
// Шаблон ApplyToMany применяет функцию f (первый аргумент) последовательно к каждому из остальных своих аргументов
template <typename F, typename...Ts>
void ApplyToMany(F& func, Ts&&... vs) {
    (..., func(std::forward<Ts>(vs)));
}

void TestSum() {
    int x;
    auto lambda = [&x](int y) {
        x += y;
    };

    x = 0;
    ApplyToMany(lambda, 1);
    assert(x == 1);

    x = 0;
    ApplyToMany(lambda, 1, 2, 3, 4, 5);
    assert(x == 15);
}

void TestConcatenate() {
    using namespace std::literals;
    std::string s;
    auto lambda = [&s](const auto& t) {
        if (!s.empty()) {
            s += " ";
        }
        s += t;
    };

    ApplyToMany(lambda, "cyan"s, "magenta"s, "yellow"s, "black"s);
    assert(s == "cyan magenta yellow black"s);
}

void TestIncrement() {
    auto increment = [](int& x) {
        ++x;
    };

    int a = 0;
    int b = 3;
    int c = 43;

    ApplyToMany(increment, a, b, c);
    assert(a == 1);
    assert(b == 4);
    assert(c == 44);
}

void TestArgumentForwarding() {
    struct S {
        int call_count = 0;
        int i = 0;
        std::unique_ptr<int> p;
        void operator()(int i) {
            this->i = i;
            ++call_count;
        }
        void operator()(std::unique_ptr<int>&& p) {
            this->p = std::move(p);
            ++call_count;
        }
    };

    S s;

    ApplyToMany(s, 1, std::make_unique<int>(42));
    assert(s.call_count == 2);
    assert(s.i == 1);
    assert(s.p != nullptr && *s.p == 42);
}

void TestArgumentForwardingToConstFunction() {
    struct S {
        mutable int call_count = 0;
        mutable int i = 0;
        mutable std::unique_ptr<int> p;
        void operator()(int i) const {
            this->i = i;
            ++call_count;
        }
        void operator()(std::unique_ptr<int>&& p) const {
            this->p = std::move(p);
            ++call_count;
        }
    };

    const S s;
    ApplyToMany(s, 1, std::make_unique<int>(42));
    assert(s.call_count == 2);
    assert(s.i == 1);
    assert(s.p != nullptr && *s.p == 42);
}

int main() {
    //Tests part 1
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
    
    //tests part 2
    TestSum();
    TestConcatenate();
    TestIncrement();
    TestArgumentForwarding();
    TestArgumentForwardingToConstFunction();
    return 0;
}

/* Напишите вашу реализацию EqualsToOneOf здесь*/
template<typename T0, typename... Ts>
bool EqualsToOneOf(const T0& v0, const Ts&... vs) {
    if constexpr(sizeof...(vs) != 0) {
        auto equals = [&v0](const T0& rhs){
            return v0 == rhs;
        };
        return (... || equals(vs));
    } else {
        return false;
    }
}

//part 3 - emplace
//#include <string>
//#include <stdexcept>
//#include <utility>

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
    template<typename... Args>
    void Emplace(Args&&... args) {
        if(is_initialized_) {
            Reset();
        }
        new (data_) T(std::forward<Args>(args)...);
        is_initialized_ = true;
    }

private:
    // alignas должен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};


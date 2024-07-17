//
//  05.Качество кода
//  Тема 03.Умные указатели
//  Урок 02.Unique_ptr
//
//  Created by Pavel Sh on 07.03.2024
//

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Cat {
    Cat(const string& name, int age)
        : name_(name)
        , age_(age)  //
    {
    }
    const string& GetName() const noexcept {
        return name_;
    }
    int GetAge() const noexcept {
        return age_;
    }
    ~Cat() {
    }
    void Speak() const {
        cout << "Meow!"s << endl;
    }

private:
    string name_;
    int age_;
};

// Функция создаёт двухлетних котов
unique_ptr<Cat> CreateCat(const string& name) {
    return make_unique<Cat>(name, 2);
}

class Witch {
public:
    explicit Witch(const string& name)
        : name_(name) {
    }

    Witch(const Witch& other)
    : name_(other.GetName())
    {
        SetCat(other.CopyCat());
    }
    
    Witch(Witch&& other)
    : name_(std::move(other.name_))
    {
        SetCat(other.ReleaseCat());
    }
    
    Witch& operator=(const Witch& other)
    {
        name_ = other.GetName();
        SetCat(other.CopyCat());
        return *this;
    }
    
    Witch& operator=(Witch&& other)
    {
        name_ = std::move(other.name_);
        SetCat(other.ReleaseCat());
        return *this;
    }
    
    const string& GetName() const noexcept {
        return name_;
    }
    void SetCat(unique_ptr<Cat>&& cat) noexcept {
        cat_ = std::move(cat);
    }
    unique_ptr<Cat> ReleaseCat() noexcept {
        return std::move(cat_);
    }
    unique_ptr<Cat> CopyCat() const {
        return std::make_unique<Cat>(cat_->GetName(), cat_->GetAge());
    }

private:
    string name_;
    unique_ptr<Cat> cat_;
};

void Test() {
    // Объекты Witch можно перемещать
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        assert(raw_cat);
        witch.SetCat(std::move(cat));

        Witch moved_witch(std::move(witch));
        auto released_cat = moved_witch.ReleaseCat();
        assert(released_cat.get() == raw_cat);  // Кот переместился от witch к moved_witch
    }

    // Можно использовать перемещающий оператор присваивания
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall");
        witch2 = move(witch);
        auto released_cat = witch.ReleaseCat();
        assert(!released_cat);
        released_cat = witch2.ReleaseCat();
        assert(released_cat.get() == raw_cat);
    }

    // Можно копировать волшебниц
    {
        Witch witch("Hermione");
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch_copy(witch);
        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // У первой волшебницы кот никуда не делся

        auto cat_copy = witch_copy.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // Копия волшебницы содержит копию кота
    }

    // Работает копирующее присваивание волшебниц
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall"s);
        witch2 = witch;

        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // У первой волшебницы кот никуда не делся

        auto cat_copy = witch2.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // При присваивании скопировался кот
    }
}

int main() {
    Test();
}

/*sln:
 struct Cat {
     Cat(const string& name, int age)
             : name_(name)
             , age_(age)  //
     {
     }
     const string& GetName() const noexcept {
         return name_;
     }
     int GetAge() const noexcept {
         return age_;
     }
     ~Cat() {
     }
     void Speak() const {
         cout << "Meow!"s << endl;
     }

 private:
     string name_;
     int age_;
 };

 // Функция создаёт двухлетних котов
 unique_ptr<Cat> CreateCat(const string& name) {
     return make_unique<Cat>(name, 2);
 }

 class Witch {
 public:
     explicit Witch(const string& name)
             : name_(name) {
     }

     Witch(const Witch& other)
             : name_(other.name_)
             , cat_(make_unique<Cat>(*other.cat_)) // Создаем копию кота
     {
     }

     Witch& operator=(const Witch& rhs) {
         if (this != &rhs) {
             name_ = rhs.name_;
             cat_ = make_unique<Cat>(*rhs.cat_); // Создаём копию кота
         }
         return *this;
     }

     // При явном объявлении конструктора копирования и копирующего оператора присваивания
     // компилятор не будет генерировать перемещающий конструктор и перемещающий оператор присваивания
     // Поэтому явно просим компилятор сгенерировать их
     Witch(Witch&&) = default;
     Witch& operator=(Witch&&) = default;

     void SetCat(unique_ptr<Cat>&& cat) noexcept {
         cat_ = std::move(cat);
     }
     unique_ptr<Cat> ReleaseCat() noexcept {
         return std::move(cat_);
     }

 private:
     string name_;
     unique_ptr<Cat> cat_;
 };
 
 */

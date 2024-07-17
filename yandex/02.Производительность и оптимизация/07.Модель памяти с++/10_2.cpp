//
//  05.Производительность и оптимизация
//  Тема 7.Модель памяти
//  Урок 10.Присваивание объектов
//
//  Created by Pavel Sh on 05.02.2024.
//

#include <algorithm>
#include <cassert>
#include <new> // Для исключения bad_alloc
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Используйте эту заготовку PtrVector или замените её на свою реализацию
template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    PtrVector(const PtrVector& other) {
        items_.reserve(other.items_.size());

        try {
            for (auto p : other.items_) {
                auto p_copy = p ? new T(*p) : nullptr;
                items_.push_back(p_copy);
            }
        } catch (...) {
            DeleteItems();
            throw;
        }
    }
    ~PtrVector() {
        DeleteItems();
    }
    // “copy-and-swap”
    PtrVector<T>& operator=(const PtrVector<T>& rhs) {
        if (this != &rhs) {
            auto rhs_copy(rhs);
            items_.swap(rhs_copy.GetItems());
        }
        return *this;
    }
    
    // Возвращает ссылку на вектор указателей
    vector<T*>& GetItems() noexcept {
        return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    vector<T*> const& GetItems() const noexcept {
        return items_;
    }

    T& at(size_t index) const {
        return *(items_.at(index));
    }
    
    size_t size() const {
        return items_.size();
    }
    std::vector<T*>::iterator begin() {
        return items_.begin();
    }
    std::vector<T*>::iterator end() {
        return items_.end();
    }
    std::vector<T*>::const_iterator begin() const {
        return items_.begin();
    }
    std::vector<T*>::const_iterator end() const {
        return items_.end();
    }
    void push_back(T* ptr) {
        items_.push_back(ptr);
    }

private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

    vector<T*> items_;
};

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id) noexcept
        : id_(id) {
    }

    int GetId() const noexcept {
        return id_;
    }

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

// Осьминог
class Octopus {
public:
    Octopus()
    : Octopus(8) {
    }
    
    explicit Octopus(int num_tentacles) {
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= num_tentacles; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const std::bad_alloc&) {
            delete t;
            throw;
        }
    }
    Octopus(const Octopus& other) {
        tentacles_ = other.tentacles_;
    }
    //destruction in the PtrVector destructor
    ~Octopus() {}
    
    Octopus& operator=(const Octopus& rhs) {
        this->tentacles_ = rhs.tentacles_;
        return *this;
    }
    
    Tentacle& AddTentacle() {
        //Реализуйте добавление щупальца самостоятельно
        int tentacle_id = static_cast<int>(tentacles_.size())+1;
        Tentacle* t = nullptr;
        try {
            t = new Tentacle(tentacle_id);
            tentacles_.push_back(t);
            t = nullptr;
        } catch (const std::bad_alloc&) {
            delete t;
            throw;
        }
        return tentacles_.at(tentacle_id-1);
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(tentacles_.size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return tentacles_.at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return tentacles_.at(index);
    }

private:
    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    PtrVector<Tentacle> tentacles_;
};


int main() {
    // Проверка присваивания осьминогов
    {
        Octopus octopus1(3);

        // Настраиваем состояние исходного осьминога
        octopus1.GetTentacle(2).LinkTo(octopus1.GetTentacle(1));

        // До присваивания octopus2 имеет своё собственное состояние
        Octopus octopus2(10);

        octopus2 = octopus1;

        // После присваивания осьминогов щупальца копии имеют то же состояние,
        // что и щупальца присваиваемого объекта
        assert(octopus2.GetTentacleCount() == octopus1.GetTentacleCount());
        for (int i = 0; i < octopus2.GetTentacleCount(); ++i) {
            auto& tentacle1 = octopus1.GetTentacle(i);
            auto& tentacle2 = octopus2.GetTentacle(i);
            assert(&tentacle2 != &tentacle1);
            assert(tentacle2.GetId() == tentacle1.GetId());
            assert(tentacle2.GetLinkedTentacle() == tentacle1.GetLinkedTentacle());
        }
    }

    // Проверка самоприсваивания осьминогов
    {
        Octopus octopus(3);

        // Настраиваем состояние осьминога
        octopus.GetTentacle(0).LinkTo(octopus.GetTentacle(1));

        vector<pair<Tentacle*, Tentacle*>> tentacles;
        // Сохраняем информацию о щупальцах осьминога и его копии
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            tentacles.push_back({&octopus.GetTentacle(i), octopus.GetTentacle(i).GetLinkedTentacle()});
        }

        // Выполняем самоприсваивание
        octopus = octopus;

        // После самоприсваивания состояние осьминога не должно измениться
        assert(octopus.GetTentacleCount() == static_cast<int>(tentacles.size()));
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            auto& tentacle_with_link = tentacles.at(i);
            assert(&octopus.GetTentacle(i) == tentacle_with_link.first);
            assert(octopus.GetTentacle(i).GetLinkedTentacle() == tentacle_with_link.second);
        }
    }
}

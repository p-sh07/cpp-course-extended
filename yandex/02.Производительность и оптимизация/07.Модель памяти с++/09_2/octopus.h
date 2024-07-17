#pragma once

// Тут можно подключить scopedptr.h и ptrvector.h,
// если они вам понадобятся.
#include "scopedptr.h"
#include "ptrvector.h"
#include <new> // Для исключения bad_alloc
#include <vector>

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
    //destruction in the PtrVector destructor
    ~Octopus() {}
    
    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
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

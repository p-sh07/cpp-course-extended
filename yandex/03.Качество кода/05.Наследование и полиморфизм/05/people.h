#pragma once

#include <iostream>
#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {
        // Реализация метода базового класса ничего не делает
    }

protected:
    
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person {
public:
    Person(const std::string& name, int age) 
    : name_(name)
    , age_(age) {}
    
    virtual ~Person() = default;

    int GetSatisfaction() const {
        return satisfaction_;
    }

    const std::string& GetName() const {
        return name_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    int GetAge() const {
        return age_;
    }

    int GetDanceCount() const {
        return dance_count_;
    }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual void Dance(int satisfaction_change = 1) {
        ++dance_count_;
        ChangeSatisfaction(satisfaction_change);
    }
    
    // Прожить день. Реализация в базовом классе ничего не делает
    virtual void LiveADay() {
        // Подклассы могут переопределить этот метод
    }
    
protected:
    int dance_count_ = 0;
    
    void ChangeSatisfaction(int points) {
        int prev_satisfaction_value = satisfaction_;
        satisfaction_ += points;
        if(observer_) {
            observer_->OnSatisfactionChanged(*this, prev_satisfaction_value, satisfaction_);
        }
    }
    
private:
    std::string name_;
    int age_;
    int satisfaction_ = 100;
    
    PersonObserver* observer_ = nullptr;
};

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person {
public:
    Worker(const std::string& name, int age)
    : Person(name, age)
    {}
    
    ~Worker() override = default;

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    void Dance(int satisfaction_change = 1) override {
        const int age = GetAge();
        Person::Dance((30 < age && age < 40) ? 2 : satisfaction_change);
    }
    // День рабочего проходит за работой
    void LiveADay() override {
        Work();
    }
    
    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        // Напишите тело метода самостоятельно
        ++work_done_;
        ChangeSatisfaction(-5);
    }

    // Возвращает значение счётчика сделанной работы
    int GetWorkDone() const {
        return work_done_;
    }
private:
    int work_done_ = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, int age)
    : Person(name, age)
    {}
    
    ~Student() override = default;

    // День студента проходит за учёбой
    void LiveADay() override {
        Study();
    }
    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        ++knowledge_level_;
        ChangeSatisfaction(-3);
    }

    // Возвращает уровень знаний
    int GetKnowledgeLevel() const {
        return knowledge_level_;
    }
private:
    int knowledge_level_ = 0;
};

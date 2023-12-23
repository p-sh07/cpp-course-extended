//
//  04.Базовые понятия С++ и STL
//  Тема 18.Стек, очередь, дек
//  Задача 02.Стек
//
//  Created by Pavel Sh on 16.12.2023.
//
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <vector>

using namespace std;
template <typename K, typename V>
ostream& operator<<(ostream& out, const pair<K,V>& pair_of_elements) {
    out << pair_of_elements.first << "[" << pair_of_elements.second << ']';
    return out;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        // напишите реализацию
        elements_.push_back(element);
    }
    void Pop() {
        // напишите реализацию
        elements_.pop_back();
    }
    const Type& Peek() const {
        // напишите реализацию
        return elements_.back();
    }
    Type& Peek() {
        // напишите реализацию
        return elements_.back();
    }
    void Print() const {
        // напишите реализацию
        for(const auto& element : elements_) {
            cout << element << ' ';
        }
        cout << endl;
    }
    uint64_t Size() const {
        // напишите реализацию
        return elements_.size();
    }
    bool IsEmpty() const {
        // напишите реализацию
        return elements_.empty();
    }
private:
    vector<Type> elements_;
};

//part 2:
template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
        // напишите реализацию метода
        if(elements_.IsEmpty()) {
            min_.Push(element);
        } else {
            min_.Push(std::min(element, min_.Peek()));
        }
        elements_.Push(element);
    }
    void Pop() {
        // напишите реализацию метода
        elements_.Pop();
        min_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        // работу этого метода мы проверять не будем,
        // но если он вам нужен, то можете его реализовать
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        // напишите реализацию метода
        return min_.Peek();
    }
    Type& PeekMin() {
        // напишите реализацию метода
        return min_.Peek();
    }
private:
    Stack<Type> elements_; // OR ! Stack<pair<type, type>>, OR Stack<PairWithMin> (struct);
    // возможно, здесь вам понадобится что-то изменить
    Stack<Type> min_;
};
//Part 3: Sack because not a real stack
template <typename Type>
class SortedSack {
public:
    void Push(const Type& element) {
        //temporary storage:
        Stack<Type> temp;
        while( !elements_.IsEmpty() && elements_.Peek() < element) {
            temp.Push(elements_.Peek());
            elements_.Pop();
        }
        elements_.Push(element);
        while(!temp.IsEmpty()) {
            elements_.Push(temp.Peek());
            temp.Pop();
        }
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
private:
    Stack<Type> elements_;
};

int main() {
    SortedSack<int> sack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего класса
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);

    // заполняем класс и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        sack.Push(values[i]);
        sack.Print();
    }
}

//part 3: using set
/*
//sack - because not a real stack
template <typename Type>
class SortedSack {
public:
    void Push(const Type& element) {
    // напишите реализацию метода
        elements_.insert(element);
    }
    void Pop() {
    // напишите реализацию метода
        elements_.erase(elements_.begin());
    }
    const Type& Peek() const {
        return *elements_.begin();
    }
    Type& Peek() {
        return *elements_.begin();
    }
    void Print() const {
        for(auto it = elements_.rbegin(); it != elements_.rend(); ++it) {
            cout << *it << ' ';
        }
        cout << endl;
    }
    uint64_t Size() const {
    return elements_.Size();
    }
    bool IsEmpty() const {
    return elements_.IsEmpty();
    }
private:
    set<Type> elements_;
};
*/

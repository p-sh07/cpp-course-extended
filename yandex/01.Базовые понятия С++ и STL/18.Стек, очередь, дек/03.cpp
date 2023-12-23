//
//  04.Базовые понятия С++ и STL
//  Тема 18.Стек, очередь, дек
//  Задача 03.Очередь и дек
//
//  Created by Pavel Sh on 18.12.2023.
//

#include <iostream>
#include <numeric>
#include <random>
#include <stack>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It start, It finish) {
    while(start != finish) {
        cout << *start << ' ';
        ++start;
    }
    cout << std::endl;
}

template <typename Type>
class Queue {
public:
    void Push(const Type& element) {
        tail_.push(element);
    }
    void Pop() {
        if(!IsEmpty()) {
            if(head_.empty()) {
                Rebalance();
            }
            head_.pop();
        }
    }
    Type& Front() {
        if(head_.empty()) {
            Rebalance();
        }
        return head_.top();
    }
    uint64_t Size() const {
        return tail_.size() + head_.size()();
    }
    bool IsEmpty() const {
        return tail_.empty() && head_.empty();
    }

private:
    stack<Type> head_;
    stack<Type> tail_;
    
    void Rebalance() {
        if(!tail_.empty()) {
            while (!tail_.empty()) {
                Type buff = tail_.top();
                head_.push(buff);
                tail_.pop();
            }
        }
    }
};

int main() {
    Queue<int> queue;
    vector<int> values(5);
    // заполняем вектор для тестирования очереди
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);
//    PrintRange(values.begin(), values.end());
    cout << "Заполняем очередь"s << endl;
    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }
    cout << "Вынимаем элементы из очереди"s << endl;
    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}


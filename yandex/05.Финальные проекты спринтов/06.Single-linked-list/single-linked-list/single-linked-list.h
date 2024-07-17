//
//  05.Производительность и оптимизация
//  Тема 8.Односвязный список
//  Урок 06.Вставка и удаление в произвольной позиции
//
//  Created by Pavel Sh on 10.02.2024.
//

#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next = nullptr)
        : value(val)
        , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;
        
        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) : node_(node) {}
        
    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора
        
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;
        
        BasicIterator() = default;
        
        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
        
        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;
        
        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            // Заглушка. Реализуйте оператор самостоятельно
            return node_ == rhs.node_;
        }
        
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
        
        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        
        // Оператор проверки итераторов на неравенство
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
        
        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }
        
        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            auto old_value = *this;
            ++(*this);
            return old_value;
        }
        
        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }
        
        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &(node_->value);
        }
        
    private:
        Node* node_ = nullptr;
    };
    
public:
//======= Constructors =======//
    SingleLinkedList() {}
    
    ~SingleLinkedList() {
        Clear();
        delete head_;
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        // Сначала надо удостовериться, что текущий список пуст (?)
        assert(size_ == 0 && head_->next_node == nullptr);
        //with pushfront: using end() because no rbegin in init_list
        auto reverse_it = values.end();
        do {
            --reverse_it;
            PushFront(*reverse_it);
        } while(reverse_it != values.begin());
    }
    
    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_->next_node == nullptr);
        
        SingleLinkedList temp;
        Node* temp_current_node = temp.head_;
        Node* other_next_node = other.head_->next_node;
        
        while(other_next_node) {
            //copy node
            temp_current_node->next_node = new Node(other_next_node->value);
            //advance to next nodes
            temp_current_node = temp_current_node->next_node;
            other_next_node = other_next_node->next_node;
        }
        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(temp);
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }
    
//======= Iterators =======//
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    
    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;
    
    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        // Реализуйте самостоятельно
        return Iterator(head_->next_node);
    }
    
    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }
    
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(head_->next_node);
    }
    
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(nullptr);
    }
    
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(head_->next_node);
    }
    
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(nullptr);
    }
    
    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        // Реализуйте самостоятельно
        return Iterator(head_);
    }
    
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(head_);
    }
    
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator(head_);
    }
    
//======= Other ========//
    // Возвращает количество элементов в списке
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
    
    // Сообщает, пустой ли список
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    // Добавляет элемент в начало списка
    void PushFront(const Type& value) {
        head_->next_node = new Node(value, head_->next_node);
        ++size_;
    }
    // Меняет содержимое списка со списком other
    void swap(SingleLinkedList& other) noexcept {
        auto tmp_node_ptr = head_->next_node;
        auto tmp_size = size_;
        
        head_->next_node = other.head_->next_node;
        size_ = other.size_;
        
        other.head_->next_node = tmp_node_ptr;
        other.size_ = tmp_size;
    }
    // Оператор копирования, по принципу copy&swap
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }
    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        //in case pos is invalid/empty
        if(!pos.node_) {
            return end();
        }
        //get the node after next
        Node* tmp_next_node_ptr = pos.node_->next_node;
        pos.node_->next_node = new Node(value,tmp_next_node_ptr);
        
        ++size_;
        
        return Iterator(pos.node_->next_node); //?
    }
    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        if(!(pos.node_->next_node)) {
            return end();
        } //get node after next
        Node* tmp_next_node_ptr = pos.node_->next_node->next_node;
        
        delete pos.node_->next_node;
        pos.node_->next_node = tmp_next_node_ptr;
        
        --size_;
        
        return Iterator(tmp_next_node_ptr); //?
    }
    // Удаляет первый элемент списка
    void PopFront() noexcept {
        EraseAfter(before_begin());
    }
    
    // Очищает память от элементов списка, кроме head_
    void Clear() noexcept {
        while (0 < size_) {
            PopFront();
        }
    }
    
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node* head_ = new Node;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    // Реализуйте обмен самостоятельно
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

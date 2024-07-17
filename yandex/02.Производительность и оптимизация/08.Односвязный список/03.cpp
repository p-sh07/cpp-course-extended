//
//  05.Производительность и оптимизация
//  Тема 8.Односвязный список
//  Урок 03.PushFront & Clear
//
//  Created by Pavel Sh on 07.02.2024.
//


#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
        : value(val)
        , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
public:
    SingleLinkedList() {}
    
    ~SingleLinkedList() {
        Clear();
    }
    // Возвращает количество элементов в списке
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
    
    // Сообщает, пустой ли список
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void Clear() noexcept {
        while (head_.next_node) {
            Node* new_next_node = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_next_node;
            --size_;
        }
    }
    
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

void Test0() {
    using namespace std;
    {
        const SingleLinkedList<int> empty_int_list;
        assert(empty_int_list.GetSize() == 0u);
        assert(empty_int_list.IsEmpty());
    }
    
    {
        const SingleLinkedList<string> empty_string_list;
        assert(empty_string_list.GetSize() == 0u);
        assert(empty_string_list.IsEmpty());
    }
}

int main() {
    Test0();
}

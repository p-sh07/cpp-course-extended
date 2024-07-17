//
//  03.Качество кода
//  Тема 03.Умные указатели
//  Урок 02.Unique_ptr, part2
//
//  Created by Pavel Sh on 07.03.2024
//

#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;  
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) noexcept {
    if (!node) {
        return true;
    }
    if ((min && node->value <= *min) || (max && node->value >= *max)) {
        return false;
    }
    return CheckTreeProperty(node->left, min, &node->value)
           && CheckTreeProperty(node->right, &node->value, max);
}

template <class T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

template <typename T>
bool CheckTreeProperty(const TreeNodePtr<T>& node_unique_ptr, const T* min, const T* max) noexcept {
    return CheckTreeProperty(node_unique_ptr.get(), min, max);
}

template <class T>
bool CheckTreeProperty(const TreeNodePtr<T>& node_unique_ptr) noexcept {
    return CheckTreeProperty<T>(node_unique_ptr.get(), nullptr, nullptr);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left.get()) {
        node = node->left.get();
    }

    return node;
}

template <typename T>
TreeNode<T>* begin(const TreeNodePtr<T>& node_unique_ptr) noexcept {
    return begin(node_unique_ptr.get());
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right.get()) {
        return begin(node->right.get());
    }
    while (node->parent) {
        bool is_right = (node == node->parent->right.get());
        if (!is_right) {
            return node->parent;
        }
        node = node->parent;
    }

    return nullptr;
}

template <typename T>
TreeNode<T>* next(const TreeNodePtr<T>& node_unique_ptr) noexcept {
    return next(node_unique_ptr.get());
}

TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    auto node = new TreeNode<int>(val, std::move(left), std::move(right));
    if (node->left) {
        node->left->parent = node;
    }
    if (node->right) {
        node->right->parent = node;
    }

    return std::unique_ptr<TreeNode<int>>(node);
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1));

    T* iter = begin(root1);
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2));
}

/*
template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;

    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNodePtr<T>& node, const T* min, const T* max) noexcept {
    if (!node) {
        return true;
    }
    if ((min && node->value <= *min) || (max && node->value >= *max)) {
        return false;
    }
    return CheckTreeProperty(node->left, min, &node->value)
           && CheckTreeProperty(node->right, &node->value, max);
}

template <class T>
bool CheckTreeProperty(const TreeNodePtr<T>& node) noexcept {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

template <typename T>
TreeNode<T>* begin(TreeNodePtr<T>& node) noexcept {
    auto node_raw_ptr = node.get();
    
    while (node_raw_ptr->left) {
        node_raw_ptr = node_raw_ptr->left.get();
    }

    return node_raw_ptr;
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left.get()) {
        node = node->left.get();
    }

    return node;
}

template <typename T>
TreeNode<T>* next(TreeNodePtr<T>& node) noexcept {
    auto node_raw_ptr = node.get();
    
    if (node_raw_ptr->right.get()) {
        return begin(node->right.get());
    }
    while (node_raw_ptr->parent) {
        bool is_right = (node_raw_ptr == node_raw_ptr->parent->right.get());
        if (!is_right) {
            return node_raw_ptr->parent;
        }
        node_raw_ptr = node_raw_ptr->parent;
    }

    return nullptr;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right) {
        return begin(node->right.get());
    }
    while (node->parent) {
        bool is_right = (node == node->parent->right.get());
        if (!is_right) {
            return node->parent;
        }
        node = node->parent;
    }

    return nullptr;
}

TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    auto node = new TreeNode<int>(val, std::move(left), std::move(right));
    if (node->left) {
        node->left->parent = node;
    }
    if (node->right) {
        node->right->parent = node;
    }

    return TreeNodePtr<int>(node);
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1));

    T* iter = begin(root1);
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2));
}

*/
 
/* task code:
template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    // Используйте TreeNodePtr<T> вместо сырых указателей.
    // Примите умные указатели по rvalue-ссылке.
    TreeNode(T val, TreeNode<T>* left, TreeNode<T>* right)
        : value(std::move(val))
        , left(left)
        , right(right) {
    }

    T value;
    TreeNode* left;  // Замените TreeNode* на TreeNodePtr<T>
    TreeNode* right; // Замените TreeNode* на TreeNodePtr<T>

    // parent оставьте обычным указателем, иначе возникнет
    // кольцевая зависимость.
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) noexcept {
    if (!node) {
        return true;
    }
    if ((min && node->value <= *min) || (max && node->value >= *max)) {
        return false;
    }
    return CheckTreeProperty(node->left, min, &node->value)
           && CheckTreeProperty(node->right, &node->value, max);
}

template <class T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left) {
        node = node->left;
    }

    return node;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right) {
        return begin(node->right);
    }
    while (node->parent) {
        bool is_right = (node == node->parent->right);
        if (!is_right) {
            return node->parent;
        }
        node = node->parent;
    }

    return nullptr;
}

// Замените указатели на умные. Сигнатура функции должна стать такой:
// TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {})
TreeNode<int>* N(int val, TreeNode<int>* left = {}, TreeNode<int>* right = {}) {
    auto node = new TreeNode<int>(val, left, right);
    if (node->left) {
        node->left->parent = node;
    }
    if (node->right) {
        node->right->parent = node;
    }

    return node;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1));

    T* iter = begin(root1);
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2));

    // Удалите вызовы функции DeleteTree.
    DeleteTree(root1);
    DeleteTree(root2);
}

*/

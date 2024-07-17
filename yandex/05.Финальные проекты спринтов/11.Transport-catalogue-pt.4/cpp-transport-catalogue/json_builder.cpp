
#include "json_builder.h"

namespace json {
///Конструктор по умолчанию, добавляет пустой Node root в tree
Builder::Builder()
: root_()
, tree_({&root_}) {
}
Builder::KeyReturnItem Builder::Key(std::string key) {
    if(!KeyExpected()) {
        throw std::logic_error("A Map Key is not expected");
    }
    //add key & store ptr to value at tree top
    auto [it, _ ] = tree_.top()->GetMap().insert({std::move(key), Node()});
    tree_.push(&it->second);
    
    return RetItm(*this);
}

Builder::RetItm Builder::Value(Node node) {
    if(!ValueExpected()) {
        throw std::logic_error("A Value is not expected");
    }
    if(!PushIfArray(node)) {
        *tree_.top() = std::move(node);
        tree_.pop();
    }
    return RetItm(*this);
}

Builder::MapReturnItem Builder::StartMap() {
    if(!ValueExpected()) {
        throw std::logic_error("Cannot make a Map");
    }
    if(!PushIfArray(Map{})) {
        tree_.top()->GetValue() = Map{};
    }
    return RetItm(*this);
}

Builder::ArrayReturnItem Builder::StartArray() {
    if(!ValueExpected()) {
        throw std::logic_error("Cannot Start an Array");
    }
    if(!PushIfArray(Array{})) {
        tree_.top()->GetValue() = Array{};
    }
    
    return RetItm(*this);
}

Builder::RetItm Builder::EndMap() {
    if(!KeyExpected() ) {
        throw std::logic_error("Cannot Finish a Map");
    }
    tree_.pop();
    return RetItm(*this);
}

Builder::RetItm Builder::EndArray() {
    if(!tree_.empty() && !tree_.top()->IsArray()) {
        throw std::logic_error("Cannot Finish an Array");
    }
    tree_.pop();
    return RetItm(*this);
}

Node Builder::Build() {
    if(!tree_.empty()) {
        throw std::logic_error("Unfinished Nodes left when calling Build");
    }
    return root_;
}

///Текущий Node - массив
bool Builder::PushIfArray(Node node) {
    if(tree_.top()->IsArray()) {
        bool push_to_stack = node.IsArray() || node.IsMap();
        tree_.top()->GetArray().push_back(std::move(node));
        
        if(push_to_stack) {
            tree_.push(&tree_.top()->GetArray().back());
        }
        return true;
    }
    return false;
}

///Ожидается значение либо словарь/массив (новый Node)
bool Builder::ValueExpected() {
    return !tree_.empty() && (tree_.top()->IsNull() || tree_.top()->IsArray());
}

///Ожидается ключ словаря
bool Builder::KeyExpected() {
    return !tree_.empty() && tree_.top()->IsMap();
}

} //namespace json


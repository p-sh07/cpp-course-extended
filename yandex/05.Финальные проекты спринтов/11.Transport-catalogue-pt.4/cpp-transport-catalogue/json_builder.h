#include "json.h"

#include <stack>
#include <string>

namespace json {

class Builder {
private:
    class RetItm;
    class KeyReturnItem;
    class MapReturnItem;
    class ArrayReturnItem;
    
public:
    ///Конструктор по умолчанию
    Builder();
    ///Передать ключ в текущий словарь
    KeyReturnItem Key(std::string key);
    ///Передать значение в текущий Node
    RetItm Value(Node node);
    ///Начать новый соварь
    MapReturnItem StartMap();
    ///Начать новый массив
    ArrayReturnItem StartArray();
    ///Завершить текущий словарь
    RetItm EndMap();
    ///Завершить текущий массив
    RetItm EndArray();
    ///Вернуть готовый Json
    Node Build();
    
private:
    bool has_nodes_ = false;
    Node root_;
    std::stack<Node*> tree_;
    
    ///Текущий Node - массив, и удалось в него положить новый Node
    bool PushIfArray(Node node);
    ///Ожидается значение либо словарь/массив (новый Node)
    bool ValueExpected();
    ///Ожидается ключ словаря
    bool KeyExpected();
    
//===================== ReturnItmems =====================//
    class RetItm {
    public:
        ///Конструктор по умолчанию
        RetItm(Builder& bd)
        : bldr_(bd) {}
        
        KeyReturnItem Key(std::string key) {
            return bldr_.Key(std::move(key));
        }
        RetItm Value(Node node) {
            return bldr_.Value(std::move(node));
        }
        MapReturnItem StartMap() {
            return bldr_.StartMap();
        }
        ArrayReturnItem StartArray() {
            return bldr_.StartArray();
        }
        RetItm EndMap() {
            return bldr_.EndMap();
        }
        RetItm EndArray() {
            return bldr_.EndArray();
        }
        Node Build() {
            return bldr_.Build();
        }
    private:
        //Builder& GetBuilder();
        Builder& bldr_;
    };

    //Возврат после добавления ключа
    class KeyReturnItem : public RetItm {
    public:
        KeyReturnItem(RetItm ret)
        : RetItm(ret) {}
        
        MapReturnItem Value(Node node) {
            return RetItm::Value(std::move(node));
        }
        
        KeyReturnItem Key(std::string key) = delete;
        RetItm EndMap() = delete;
        RetItm EndArray() = delete;
        Node Build() = delete;
    };
    //Возврат после создания словаря
    class MapReturnItem : public RetItm {
    public:
        MapReturnItem(RetItm ret)
        : RetItm(ret) {}
        
        RetItm Value(Node node) = delete;
        MapReturnItem StartMap() = delete;
        ArrayReturnItem StartArray() = delete;
        RetItm EndArray() = delete;
        Node Build() = delete;
    };
    //Возврат после создания массива
    class ArrayReturnItem : public RetItm {
    public:
        ArrayReturnItem(RetItm ret)
        : RetItm(ret) {}
        
        ArrayReturnItem Value(Node node) {
            return RetItm::Value(std::move(node));
        }

        KeyReturnItem Key(std::string key) = delete;
        RetItm EndMap() = delete;
        Node Build() = delete;
    };
};

} //namespace json

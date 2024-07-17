//
//  05.Производительность и оптимизация
//  Тема 09.Эффективные линейные контейнеры
//  Урок 08.Эффективный список - текстовый редактор
//
//  Created by Pavel Sh on 14.02.2024.
//

#include <iostream>
#include <list>
#include <string>

using std::list;
using std::string;
using std::operator""s;

class Editor {
public:
    Editor() {}
    // сдвинуть курсор влево
    void Left() {
        if(cursor_ != text_.begin()) {
            --cursor_;
        }
    }
    // сдвинуть курсор вправо
    void Right(){
        if(cursor_ != text_.end()) {
            ++cursor_;
        }
    }
    // вставить символ token
    void Insert(char token) {
        text_.insert(cursor_, token);
    }
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        iter start_cut = cursor_;
        //move cursor to the end of cut, so that it stays in the text_
        cursor_ = GetNextIt(cursor_, tokens);
        buff_.assign(start_cut, cursor_);
        text_.erase(start_cut, cursor_);
    }
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        iter end_copy = GetNextIt(cursor_, tokens);
        buff_.assign(cursor_, end_copy);
    }
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text_.insert(cursor_, begin(buff_), end(buff_));
    }
    // получить текущее содержимое текстового редактора
    string GetText() const {
        return {begin(text_), end(text_)};
    }
private:
    using iter = list<char>::iterator;
    
    list<char> text_;
    list<char> buff_;
    iter cursor_ = text_.begin();
    
    iter GetNextIt(iter start, size_t num = 1) {
        while(start != text_.end() && 0 < num) {
            ++start;
            --num;
        }
        return start;
    }
};

int main() {
    Editor editor;
    const std::string text = "hello, world"s;
    editor.Left();
    editor.Right();
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    std::cout << editor.GetText() << std::endl;
    return 0;
}

//
//  05.Производительность и оптимизация
//  Тема 3.Работа с файлами
//  Задача 04.Тонкости открытия файлов
//
//  Created by Pavel Sh on 25.12.2023.
//
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// реализуйте эту функцию:
size_t GetFileSize(string file);

int main() {
    ofstream("test.txt") << "123456789"s;
    assert(GetFileSize("test.txt"s) == 9);

    ofstream test2("test2.txt"s);
    test2.seekp(1000);
    test2 << "abc"s;
    test2.flush();

    assert(GetFileSize("test2.txt"s) == 1003);
    assert(GetFileSize("missing file name"s) == string::npos);
}

size_t GetFileSize(string file) {
    ifstream fs(file);
    if(!fs.is_open()) {
        return string::npos;
    }
    fs.seekg(0, fs.end);
    size_t sz = fs.tellg();
    
    return sz;
}

/*
 неудобно, поэтому у seekp есть второй параметр, обозначающий точку отсчёта. Он может принимать следующие значения:
 ios::beg — отсчитывать от начала файла,
 ios::end — отсчитывать от конца файла,
 ios::cur — отсчитывать от текущего места.
 При использовании ios::end и ios::cur допустимо отрицательное значение позиции. Например, если мы хотим сместиться на десять символов назад, можно написать stream.seekp(-10, ios::cur). Перейти при записи в самый конец файла можно командой stream.seekp(0, ios::end). 
 */

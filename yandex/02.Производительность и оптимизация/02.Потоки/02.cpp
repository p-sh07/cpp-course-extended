//
//  05.Производительность и оптимизация
//  Тема 2.Потоки
//  Задача 02.Такие разные потоки
//
//  Created by Pavel Sh on 24.12.2023.
//
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

// реализуйте эту функцию:
void CreateFiles(const string& config_file) {
    ifstream config(config_file);
    ofstream out;
    string input;
    bool file_open = false;
    while(getline(config,input)) {
        if(input.empty()) {
            throw;
        }
        if(input[0] == '>') {
            out << input.substr(1) << '\n';
        } else {
            if(file_open) {
                out.close();
            }
            out.open(input);
            file_open = true;
        }
    }
    out.close();
}

string GetLine(istream& in) {
    string s;
    getline(in, s);
    return s;
}

int main() {
    CreateFiles("config.txt"s);
}

//
//  04.Базовые понятия С++ и STL
//  Тема 16.Итераторы
//  Задача 02.Понятие итератора -> до задачи
//
//  Created by Pavel Sh on 11.12.2023.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};
//2
int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    // Выведите первый язык, начинающийся на J, используя алгоритм find_if
    auto it = find_if(langs.begin(), langs.end(), [](const Lang& lang) {
        return !lang.name.empty() && lang.name[0] == 'J';
    });itera
    cout << it->name << ", " << it->age << endl;
}
//3 Print Range
template <typename It>
void PrintRange(It start, It finish) {
    while(start != finish) {
        cout << *start << ' ';
        ++start;
    }
    cout << std::endl;
}
//3.2 Make vector
template <typename It>
auto MakeVector (It start, It finish) {
    return vector(start, finish);
}
//4 Print before and after find on two lines
template <typename Container, typename Element>
void FindAndPrint(const Container& container, const Element& element_to_find) {
    auto start = container.begin();
    auto finish = container.end();
    auto found_element = find(start, finish, element_to_find);
    PrintRange(start, found_element);
    PrintRange(found_element, finish);
}
//5.1 Erase element and print
template <typename Container, typename Element_It>
void EraseAndPrint(Container& container, Element_It iterator_to_element) {
    auto pos_after_element = container.erase(iterator_to_element);
    PrintRange(container.begin(), pos_after_element);
    PrintRange(pos_after_element,  container.end());
}
//5.2 Erase element and print (using element position)
template <typename Container>
void EraseAndPrint(Container& container, int position_of_element) {
    auto pos_after_element = container.erase(std::next(container.begin(), position_of_element));
    PrintRange(container.begin(), pos_after_element);
    PrintRange(pos_after_element,  container.end());
}
//5.3 Первое число — позиция, которая должна быть удалена. Второе и третье число — полуинтервал позиций в полученном после удаления контейнере. Они тоже должны быть удалены. Выведите на экран сначала контейнер после удаления первой позиции, а потом после удаления диапазона.
template <typename Container>
void EraseAndPrint(Container& container, int position_of_element, int erase_range_start, int erase_range_end) {
    container.erase(std::next(container.begin(), position_of_element));
    PrintRange(container.begin(), container.end());
    container.erase(std::next(container.begin(), erase_range_start), std::next(container.begin(), erase_range_end));
    PrintRange(container.begin(), container.end());
}

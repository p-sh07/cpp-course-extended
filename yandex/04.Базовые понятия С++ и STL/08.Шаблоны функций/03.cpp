//
//  04.Базовые понятия С++ и STL
//  Тема 08.Шаблоны функций
//  Задача 03.Обобщаем функцию
//
//  Created by Pavel Sh on 28.11.2023.
//

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}
//sort by freq + lexical
pair<string, int> FindMaxFreqAnimal(const vector<pair<string, int>>& animals) {
    const auto frequencies = ComputeTermFreqs(animals);
    pair<string, int> most_freq_animal;
    int max_count = 0;
    for(const auto& [animal, count] : frequencies) {
        if (count > max_count) {
            most_freq_animal = animal;
            max_count = count;
        }
    }
    return most_freq_animal;
}

int main() {
    const vector<pair<string, int>> animals = {
        {"Murka"s, 5},  // 5-летняя Мурка
        {"Belka"s, 6},  // 6-летняя Белка
        {"Murka"s, 7},  // 7-летняя Мурка не та же, что 5-летняя!
        {"Murka"s, 5},  // Снова 5-летняя Мурка
        {"Belka"s, 6},  // Снова 6-летняя Белка
    };
    const pair<string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    cout << max_freq_animal.first << " "s << max_freq_animal.second << endl;
}

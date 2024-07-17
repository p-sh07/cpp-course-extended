//
//  03.Производительность и оптимизация
//  Тема 02.Пространство имен
//  Урок 02.Использование namespace, part 1
//
//  Created by Pavel Sh on 28.02.2024
//

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

// не меняйте файлы json.h и json.cpp
#include "json.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(
        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
            return current + spending.amount;
        });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    // место для вашей реализации
    const string cat_label = "category";
    const string amnt_label = "amount";
    //results
    vector<Spending> spent;
    Document doc = Load(input);
    
    for(const auto& item : doc.GetRoot().AsArray()) {
            spent.push_back({item.AsMap().at(cat_label).AsString(), item.AsMap().at(amnt_label).AsInt()});
        }
    }
    return spent;
}
//int main() {
//    // не меняйте main
//    ifstream in_txt("json.txt");
//    const vector<Spending> spendings = LoadFromJson(in_txt);
//    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
//    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
//}


int main() {
    // не меняйте main
    const vector<Spending> spendings = LoadFromJson(cin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}

/*
[
  {"amount": 2500, "category": "food"},
  {"amount": 1150, "category": "transport"},
  {"amount": 5780, "category": "restaurants"},
  {"amount": 7500, "category": "clothes"},
  {"amount": 23740, "category": "travel"},
  {"amount": 12000, "category": "sport"}
]
*/

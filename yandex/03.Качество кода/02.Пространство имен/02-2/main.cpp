//
//  03.Производительность и оптимизация
//  Тема 02.Пространство имен
//  Урок 02.Использование namespace, part 2
//
//  Created by Pavel Sh on 28.02.2024
//

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// не меняйте файлы xml.h и xml.cpp
#include "xml.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(spendings.begin(), spendings.end(), 0,
                      [](int current, const Spending& spending) {
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

vector<Spending> LoadFromXml(istream& input) {
    string cat_label = "category";
    string amnt_label = "amount";

    Document doc = Load(input);
    vector<Spending> spent;
    
    for(const Node& item : doc.GetRoot().Children()) {
        spent.push_back({item.AttributeValue<string>(cat_label), item.AttributeValue<int>(amnt_label)});
    }
    return spent;
}

int main() {
    ifstream in_xml("xml.txt");
    const vector<Spending> spendings = LoadFromXml(in_xml);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}

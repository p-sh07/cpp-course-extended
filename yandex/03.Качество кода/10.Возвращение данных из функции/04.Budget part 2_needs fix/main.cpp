#include "budget_manager.h"

#include <iostream>
#include <fstream>
#include <string_view>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    BudgetQuery q = BudgetParser::ParseQuery(line);
    if(q.type == QueryType::ComputeIncome) {
        cout << manager.ComputeIncome(q) << '\n';
    } else {
        manager.ProcessQuery(q);
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;
    
    ifstream in("/Users/ps/Documents/cpp-course/input_files/in.txt");
    //istream& in = cin;
    const int query_count = ReadNumberOnLine(in);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(in, line);
        ParseAndProcessQuery(manager, line);
    }
}

#include "parser.h"

#include <iostream>
#include <fstream>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    auto query = ParseQuery(line);

    if (!query) {
        return;
    }

    query->ProcessAndPrint(manager, cout);
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;

    ifstream in("/Users/ps/Documents/cpp-course/input_files/in.txt");
    const int query_count = ReadNumberOnLine(in);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(in, line);
        ParseAndProcessQuery(manager, line);
    }
}

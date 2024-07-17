#pragma once
#include <iostream>
#include <string_view>
#include "date.h"

enum class QueryType {
    ComputeIncome,
    Earn,
    PayTax,
    Spend,
    Unknown,
};

struct DateInterval {
    Date first;
    Date last;
};

struct BudgetQuery {
    QueryType type;
    DateInterval dates;
    double sum;
    double daily;
    int tax_percent;
};

QueryType StrToQtype(std::string_view qtype);

// напишите в этом классе код, ответственный за чтение запросов
class BudgetParser{
public:
    static BudgetQuery ParseQuery(std::string_view q);
};

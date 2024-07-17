// разработайте менеджер личного бюджета самостоятельно
#include "parser.h"

#include <stdexcept>
#include <ranges>
#include <sstream>
#include <string>

using namespace std::literals;
namespace rng = std::ranges;
namespace view = std::views;

using std::string_view;

QueryType StrToQtype(string_view q) {
    if(q == "ComputeIncome"sv) {
        return QueryType::ComputeIncome;
    }
    else if (q == "Earn"sv) {
        return QueryType::Earn;
    }
    else if (q == "PayTax"sv) {
        return QueryType::PayTax;
    }
    else if (q == "Spend"sv) {
        return QueryType::Spend;
    }
    return QueryType::Unknown;
}

BudgetQuery BudgetParser::ParseQuery(string_view query) {
    
    auto split_query = query | view::split(' ')
    | view::transform([](auto r) {
        return string_view(r.begin(), r.end());
    });
    
    if(split_query.empty()) {
        throw std::runtime_error("Failed to parse Query: query Split empty");
    }
    //TODO
    //    rng::subrange split(split_query);
    //
    //    QueryType qtype = StrToQtype(rng::get<0>(split));
    //    auto date_first = Date::FromString(rng::get<1>(split_query));
    //    auto date_last = Date::FromString(rng::get<2>(split_query));
    auto split_query_it = split_query.begin();
    
    QueryType qtype =  StrToQtype(*std::next(split_query_it, 0));
    auto date_first = Date::FromString(*std::next(split_query_it, 1));
    auto date_last = Date::FromString(*std::next(split_query_it, 2));
    
    
    double sum = -1.0;
    int tax = -1;
    
    
    //Read number if query includes one
    if(qtype == QueryType::PayTax && std::next(split_query_it, 3) != split_query.end()) {
        //std::cerr << "Computing tax from str: " << *std::next(split_query_it, 3) << std::endl;
        tax = std::stoi(std::string(*std::next(split_query_it, 3)));
    }
    else if(qtype == QueryType::Earn || qtype == QueryType::Spend) {
        sum = std::stod(std::string(*std::next(split_query_it, 3)));
    }
    
    return { qtype, {date_first, date_last}, sum, 0.0, tax };
}
/*
 
 ComputeIncome <дата начала> <дата конца> — вычислить чистую прибыль за данный диапазон дат и вывести результат в cout.
 Earn <дата начала> <дата конца> <прибыль> — учесть, что в этот период равномерно по дням была заработана указанная сумма. Прибыль — произвольное положительное число double.
 PayTax <дата начала> <дата конца> — заплатить налог 13% в каждый день указанного диапазона. Это означает простое умножение всей прибыли в диапазоне на 0,87, независимо от того, отдавался ли уже налог за какой-либо из указанных дней. Прибыль за эти дни, которая обнаружится позже, налогами из прошлого не облагается.
 */

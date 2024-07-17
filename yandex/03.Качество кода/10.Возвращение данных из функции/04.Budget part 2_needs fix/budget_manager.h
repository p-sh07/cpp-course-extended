#pragma once
#include <algorithm>
#include <ranges>
#include <vector>
#include <string_view>

#include "parser.h"
#include "date.h"

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};
    
    inline static const int DEFAULT_TAX = 13;
    
    struct BudgetData {
        double earned;
        double spent;
    };
    
    // разработайте класс BudgetManager
    BudgetManager()
    : data_(Date::ComputeDistance(START_DATE, END_DATE), {0.0, 0.0}) {
    }
    
    double ComputeIncome(const BudgetQuery& q) const;

    void ProcessQuery(BudgetQuery q);

    template<typename F>
    void ProcessInputQuery(BudgetQuery q, F func);

private:
    //Inclusive of first & last date
    inline auto GetDateRange(const DateInterval& dates)  {
        return std::ranges::subrange(std::next(data_.begin(), GetDayId(dates.first)), std::next(data_.begin(), GetDayId(dates.last) + 1));
    }
    inline auto GetDateRange(const DateInterval& dates) const  {
        return std::ranges::subrange(std::next(data_.begin(), GetDayId(dates.first)), std::next(data_.begin(), GetDayId(dates.last) + 1));
    }
    
    inline void Earn(const BudgetQuery& q, BudgetData& b) {
        b.earned += q.daily;
    }

    inline void PayTax(const BudgetQuery& q, BudgetData& b) {
        b.earned *= (1 - (q.tax_percent < 0 ? DEFAULT_TAX : q.tax_percent) /100.0);
    }

    inline void Spend(const BudgetQuery& q, BudgetData& b) {
            b.spent += q.daily;
    }
    
    
    size_t GetDayId(const Date& day) const;
    //construct a vector of length DAYS (all specified dates)
    std::vector<BudgetData> data_;
};

template<typename F>
void BudgetManager::ProcessInputQuery(BudgetQuery q, F func) {
//   doesn't compile... std::ranges::for_each(GetDateRange(q.dates), [&q, &func](const BudgetData& b) { func(q,b); });
    //using for:
    if(q.type == QueryType::Earn || q.type == QueryType::Spend) {
        q.daily = q.sum / (1.0 * GetDateRange(q.dates).size());
    }
    
    for(auto& b : GetDateRange(q.dates)) {
        func(q, b);
    }
}

/*еред вами первая часть. Программа должна поддерживать запросы трёх типов:
Поступление средств на счёт за определённый период. Распределяет средства равномерно по дням.
Уплата налога 13%. Вычитает 13 процентов из совокупного дохода каждого дня указанного периода, независимо от того, платились ли уже в этот день налоги.
Запрос на выведение дохода. Печатает в cout чистую прибыль всех дней на данный момент.
*/

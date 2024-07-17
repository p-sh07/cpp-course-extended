// разработайте менеджер личного бюджета самостоятельно
#include "budget_manager.h"

#include <functional>
#include <numeric>
using namespace std::literals;
namespace rng = std::ranges;
namespace view = std::views;

size_t BudgetManager::GetDayId(const Date& day) const {
    return static_cast<size_t>(Date::ComputeDistance(START_DATE, day));
}

void BudgetManager::ProcessQuery(BudgetQuery q) {
    switch (q.type) {
        case QueryType::ComputeIncome:
            throw std::logic_error("Cannot process ComputeIncome query, incorrect function called");
            break;
        case QueryType::Earn:
            ProcessInputQuery(std::move(q), std::bind(&BudgetManager::Earn, this, std::placeholders::_1, std::placeholders::_2));
            break;
        case QueryType::PayTax:
            ProcessInputQuery(q, std::bind(&BudgetManager::PayTax, this, std::placeholders::_1, std::placeholders::_2));
            break;
        case QueryType::Spend:
            ProcessInputQuery(std::move(q),std::bind(&BudgetManager::Spend, this, std::placeholders::_1, std::placeholders::_2));
            break;
            
        default:
            break;
    }
}

double BudgetManager::ComputeIncome(const BudgetQuery& q) const {
    const auto range = GetDateRange(q.dates);
    //std::cerr << "From day " << GetDayId(q.dates.first) << '/' << range.begin() - data_.begin() << " income:" << range.begin()->earned << std::endl;
    
    //std::cerr << "To day " << GetDayId(q.dates.last) << '/' << std::prev(range.end()) - data_.begin() << " income:" << std::prev(range.end())->earned << std::endl;
    
    return std::accumulate(range.begin(), range.end(), 0.0,
        [](double sum, const BudgetData& b) {
            return sum += (b.earned - b.spent);
    });
}

////std::cerr << "Income from days: " << std::distance(income_.begin(), dates_rng.first) << " & " << std::distance(income_.begin(), dates_rng.second) << '\n';

////std::cerr << "Adding to days: " << std::distance(income_.begin(), dates_rng.first) << " & " << std::distance(income_.begin(), dates_rng.second) << " Sum: " << daily_earn << '\n';

////std::cerr << "Paying tax: " << std::distance(income_.begin(), dates_rng.first) << " & " << std::distance(income_.begin(), dates_rng.second) << '\n';

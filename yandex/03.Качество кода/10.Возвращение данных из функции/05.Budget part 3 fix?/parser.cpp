#include "parser.h"

using namespace std;

namespace queries {
class ComputeIncome : public ComputeQuery {
public:
    using ComputeQuery::ComputeQuery;
    ReadResult Process(const BudgetManager& budget) const override {
        return {budget.ComputeSum(GetFrom(), GetTo())};
    }
    
    
};

class Alter : public ModifyQuery {
public:
    Alter(Date from, Date to, double amount, bool spend = false)
    : ModifyQuery(from, to)
    , amount_(amount) 
    , spend_(spend) {
    }
    
    void Process(BudgetManager& budget) const override {
        double day_income = amount_ / (Date::ComputeDistance(GetFrom(), GetTo()) + 1);
        
        budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneyAdder{day_income}, spend_);
    }
    
private:
    double amount_;
    bool spend_;
};

class PayTax : public ModifyQuery {
public:
    PayTax(Date from, Date to, int tax_percent)
    : ModifyQuery(from, to)
    , tax_percent_(tax_percent) {
    }
    
    void Process(BudgetManager& budget) const override {
        budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{tax_percent_}, false);
    }
    
private:
    const int tax_percent_;
};

}  // namespace queries

std::unique_ptr<Query> ParseQuery(std::string_view line)
{
    using namespace queries;
   auto [id, pconfig] = SplitFirst(line, ' ');
   
   if(!pconfig.has_value()) {
       throw std::runtime_error("no command config found by parser"s);
   }
   
   auto parts = Split(pconfig.value(), ' ');
   
   if(id == "ComputeIncome"sv) {
       return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
   } else if (id == "Earn"sv) {
       return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), std::stod(std::string(parts[2])));
   }
   else if (id == "Spend"sv) {
       return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), std::stod(std::string(parts[2])), true);
   }
   else if (id == "PayTax"sv) {
       return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), std::stoi(std::string(parts[2])));
   }
   return nullptr;
}

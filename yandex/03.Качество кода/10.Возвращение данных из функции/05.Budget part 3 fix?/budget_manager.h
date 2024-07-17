#pragma once
#include "bulk_updater.h"
#include "date.h"
#include "entities.h"

#include <vector>
#include <iostream>

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};

    static size_t GetDayIndex(Date day) {
        return static_cast<size_t>(Date::ComputeDistance(START_DATE, day));
    }

    static IndexSegment MakeDateSegment(Date from, Date to) {
        return {GetDayIndex(from), GetDayIndex(to) + 1};
    }

    double ComputeSum(Date from, Date to) const {
        auto date_segment = MakeDateSegment(from, to);
        //std::cerr << "Earn tree sum: " << tree_.ComputeSum(date_segment) << " Spend sum: " << spend_tree_.ComputeSum(date_segment) << std::endl;
        return (tree_.ComputeSum(date_segment) - spend_tree_.ComputeSum(date_segment));
    }

    void AddBulkOperation(Date from, Date to, const BulkLinearUpdater& operation, bool spend) {
        if(spend) {
            spend_tree_.AddBulkOperation(MakeDateSegment(from, to), operation);
        } else {
            tree_.AddBulkOperation(MakeDateSegment(from, to), operation);
        }
    }

private:
    SummingSegmentTree<double, BulkLinearUpdater> tree_{GetDayIndex(END_DATE)};
    SummingSegmentTree<double, BulkLinearUpdater> spend_tree_{GetDayIndex(END_DATE)};
};

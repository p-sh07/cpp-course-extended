#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>
#include <iostream>

struct BulkMoneyAdder {
    double delta = {};
};

struct BulkTaxApplier {
    BulkTaxApplier(int tax_percent = 0)
    : factor(1.0 - (1.0 * tax_percent / 100.0)) {
    }
    double ComputeFactor() const {
        return std::pow(factor, count);
    }
    double factor;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
           // std::cerr << " -Adding tax: " << tax_.tax_percent << '\n';
    }

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta = add_.delta * other.tax_.factor + other.add_.delta;
    }

    double Collapse(double origin, IndexSegment segment) const {
        return origin * tax_.ComputeFactor() + add_.delta * static_cast<double>(segment.length());
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};

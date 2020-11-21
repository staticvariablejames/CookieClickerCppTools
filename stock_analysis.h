/* stock_analysis.h
 * Algorithms for analyzing stock behaviors.
 */
#ifndef STOCK_ANALYSIS_H
#define STOCK_ANALYSIS_H

#include <ostream>
#include <vector>

#include "stock.h"

namespace CCCPP {

struct histogram {
    int stock_id;
    int bank_level;
    std::vector<int> data;
    static constexpr int year_of_ticks = 365*24*60 + 24*60/4;
};

// Outputs Javascript code to assign the histogram to InsugarTrading.data[level][id].
std::ostream& operator<<(std::ostream& os, const histogram&);

template<typename RNG>
histogram generateStockHistogram(Stock& stock, RNG& rng, int ticks) {
    histogram h;
    h.stock_id = stock.id();
    h.bank_level = stock.bank_level();
    for(auto i = 0; i < ticks; i++) {
        stock.tick(rng);
        auto index = static_cast<int>(10*stock.value());
        if(h.data.size() <= index) h.data.resize(index+1);
        h.data[index]++;
    }
    return h;
}

} // namespace CCCPP

#endif // STOCK_ANALYSIS_H

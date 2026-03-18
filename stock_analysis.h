/* stock_analysis.h
 * Algorithms for analyzing stock behaviors.
 */
#ifndef STOCK_ANALYSIS_H
#define STOCK_ANALYSIS_H

#include <array>
#include <ostream>
#include <vector>

#include "stock.h"

namespace CCCPP {

struct histogram_set {
    static constexpr size_t dragonBoostCount = 4;
    static constexpr std::array<double, dragonBoostCount> dragonBoosts = {0.0, 0.1, 1.0, 1.1};

    int stock_id;
    int bank_level;
    std::array<std::vector<int>, dragonBoostCount> data;

    static constexpr int year_of_ticks = 365*24*60 + 24*60/4;
};

// Outputs Javascript code to assign the histogram to InsugarTrading.data[level][id].
std::ostream& operator<<(std::ostream& os, const histogram_set&);

template<typename RNG>
histogram_set generateStockHistogram(Stock& stock, RNG& rng, int ticks) {
    histogram_set h;
    h.stock_id = stock.id();
    h.bank_level = stock.bank_level();
    for(auto boost_index = 0; boost_index < histogram_set::dragonBoostCount; boost_index++) {
        const auto dragonBoost = histogram_set::dragonBoosts[boost_index];
        for(auto i = 0; i < ticks; i++) {
            stock.tick(rng, dragonBoost);
            auto index = static_cast<int>(10*stock.value());
            if(h.data[boost_index].size() <= index) h.data[boost_index].resize(index+1);
            h.data[boost_index][index]++;
        }
    }
    return h;
}

} // namespace CCCPP

#endif // STOCK_ANALYSIS_H

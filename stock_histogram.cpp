#include "stock_histogram.h"

namespace CCCPP {

constexpr std::array<const char*, histogram_set::dragonBoostCount> boostStr = {"0.0", "0.1", "1.0", "1.1"};

std::ostream& operator<<(std::ostream& os, const histogram_set& h) {
    for(auto boost_index = 0; boost_index < histogram_set::dragonBoostCount; boost_index++) {
        os << "InsugarTrading.data[" << h.bank_level << "]"
                                 "[" << boostStr[boost_index] << "]"
                                 "[" << h.stock_id << "] = [";
        auto sep = "";
        for(const auto & i : h.data[boost_index]) {
            os << sep << i;
            sep = ",";
        }
        os << "];\n";
    }
    return os;
}

} // namespace CCCPP

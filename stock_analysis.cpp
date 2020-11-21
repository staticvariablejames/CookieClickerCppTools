#include "stock_analysis.h"

namespace CCCPP {

std::ostream& operator<<(std::ostream& os, const histogram& h) {
    os << "InsugarTrading.data[" << h.bank_level << "]"
                             "[" << h.stock_id << "] = [";
    auto sep = "";
    for(const auto & i : h.data) {
        os << sep << i;
        sep = ",";
    }
    os << "];";
    return os;
}

} // namespace CCCPP

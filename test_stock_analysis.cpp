#include "stock_analysis.h"
#include "catch2/catch.hpp"

#include <iostream>

#include "random.h"

using namespace CCCPP;

TEST_CASE("stockHistogram works", "[stock]") {
    auto rng = prng("test");
    auto stock = Stock(0, 1, rng);
    stock.value(1);

    auto data = std::vector<int>(51);
    data[30] = 1;
    data[40] = 1;
    data[43] = 1;
    data[45] = 1;
    data[48] = 3;
    data[49] = 1;
    data[50] = 2;
    
    auto h = generateStockHistogram(stock, rng, 10);
    CHECK(h.stock_id == 0);
    CHECK(h.bank_level == 1);
    CHECK(h.data == data);
}

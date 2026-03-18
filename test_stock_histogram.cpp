#include "stock_histogram.h"
#include "catch2/catch.hpp"

#include <sstream>

#include "random.h"

using namespace CCCPP;

TEST_CASE("generateStockHistogram works", "[stock]") {
    auto rng = prng("histogram test");
    Stock stock;
    stock.value(1);

    auto data = std::array<std::vector<int>, 4>{
        std::vector<int>(51),
        std::vector<int>(49),
        std::vector<int>(107),
        std::vector<int>(77),
    };

    data[0][30] = 1;
    data[0][38] = 1;
    data[0][41] = 2;
    data[0][45] = 1;
    data[0][46] = 2;
    data[0][48] = 1;
    data[0][49] = 1;
    data[0][50] = 1;

    data[1][29] = 1;
    data[1][31] = 1;
    data[1][34] = 1;
    data[1][36] = 2;
    data[1][39] = 1;
    data[1][44] = 3;
    data[1][48] = 1;

    data[2][43] = 1;
    data[2][49] = 2;
    data[2][52] = 1;
    data[2][95] = 2;
    data[2][101] = 3;
    data[2][106] = 1;

    data[3][21] = 1;
    data[3][24] = 1;
    data[3][26] = 1;
    data[3][31] = 1;
    data[3][32] = 1;
    data[3][35] = 1;
    data[3][38] = 1;
    data[3][41] = 1;
    data[3][57] = 1;
    data[3][76] = 1;

    auto h = generateStockHistogram(stock, rng, 10);
    CHECK(h.stock_id == 0);
    CHECK(h.bank_level == 1);
    REQUIRE(h.data[0].size() == data[0].size());
    REQUIRE(h.data[1].size() == data[1].size());
    REQUIRE(h.data[2].size() == data[2].size());
    REQUIRE(h.data[3].size() == data[3].size());
    CHECK(h.data[0] == data[0]);
    CHECK(h.data[1] == data[1]);
    CHECK(h.data[2] == data[2]);
    CHECK(h.data[3] == data[3]);
}

TEST_CASE("histogram operator<< works", "[stock]") {
    auto h = histogram_set{
        .stock_id = 3,
        .bank_level = 7,
        .data = {
            std::vector<int>{0, 0, 0, 1, 2, 3, 4},
            std::vector<int>{0, 0, 1, 2, 3, 4, 0},
            std::vector<int>{0, 1, 2, 3, 4, 0, 0},
            std::vector<int>{1, 2, 3, 4, 0, 0, 0},
        },
    };
    std::stringstream ss;
    ss << h;
    CHECK(ss.str() ==
            "InsugarTrading.data[7][0.0][3] = [0,0,0,1,2,3,4];\n"
            "InsugarTrading.data[7][0.1][3] = [0,0,1,2,3,4,0];\n"
            "InsugarTrading.data[7][1.0][3] = [0,1,2,3,4,0,0];\n"
            "InsugarTrading.data[7][1.1][3] = [1,2,3,4,0,0,0];\n"
        );
}

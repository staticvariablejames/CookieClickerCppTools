#include "stock.h"
#include "catch2/catch.hpp"

using namespace CCCPP;

TEST_CASE("Basic getters and setters of CCCPP::stock work", "[stock]") {
    Stock stock;
    CHECK(stock.value() == Approx(10.0));
    CHECK(stock.delta() == Approx(0.0));
    CHECK(stock.mode() == StockMode::Stable);
    CHECK(stock.duration() == 10);
    CHECK(stock.id() == 0);
    CHECK(stock.bank_level() == 1);
    CHECK(stock.restingValue() == Approx(10.0));
    CHECK(stock.softCap() == Approx(100.0));

    stock.value(32.0);
    stock.delta(-1.5);
    stock.mode(StockMode::FastRise);
    stock.duration(550);
    stock.id(3);
    stock.bank_level(5);

    CHECK(stock.value() == Approx(32.0));
    CHECK(stock.delta() == Approx(-1.5));
    CHECK(stock.mode() == StockMode::FastRise);
    CHECK(stock.duration() == 550);
    CHECK(stock.id() == 3);
    CHECK(stock.bank_level() == 5);
    CHECK(stock.restingValue() == Approx(44.0));
    CHECK(stock.softCap() == Approx(112.0));
}

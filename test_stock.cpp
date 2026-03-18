#include "stock.h"
#include "catch2/catch.hpp"

#include <iostream>

#include "random.h"

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

    CHECK(stock.javascriptString() ==
        "Game.Objects['Bank'].minigame.goodsById[0].val=10;"
        "Game.Objects['Bank'].minigame.goodsById[0].d=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].mode=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].dur=10;"
    );
    CHECK(stock.javascriptString(true) ==
        "Game.Objects['Bank'].minigame.goodsById[0].val=10;"
        "Game.Objects['Bank'].minigame.goodsById[0].d=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].mode=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].dur=10;"
        "Game.Objects['Bank'].level=1;"
    );

    stock.value(32.111111111111111111);
    stock.delta(-1.111111111111111111);
    stock.mode(StockMode::FastRise);
    stock.duration(550);
    stock.id(3);
    stock.bank_level(5);

    CHECK(stock.value() == Approx(32.1111111111111111));
    CHECK(stock.delta() == Approx(-1.1111111111111111));
    CHECK(stock.mode() == StockMode::FastRise);
    CHECK(stock.duration() == 550);
    CHECK(stock.id() == 3);
    CHECK(stock.bank_level() == 5);
    CHECK(stock.restingValue() == Approx(44.0));
    CHECK(stock.softCap() == Approx(112.0));

    CHECK(stock.javascriptString() ==
        "Game.Objects['Bank'].minigame.goodsById[3].val=32.111111111111114;" // precision
        "Game.Objects['Bank'].minigame.goodsById[3].d=-1.1111111111111112;"
        "Game.Objects['Bank'].minigame.goodsById[3].mode=3;"
        "Game.Objects['Bank'].minigame.goodsById[3].dur=550;"
    );
    CHECK(stock.javascriptString(true) ==
        "Game.Objects['Bank'].minigame.goodsById[3].val=32.111111111111114;"
        "Game.Objects['Bank'].minigame.goodsById[3].d=-1.1111111111111112;"
        "Game.Objects['Bank'].minigame.goodsById[3].mode=3;"
        "Game.Objects['Bank'].minigame.goodsById[3].dur=550;"
        "Game.Objects['Bank'].level=5;"
    );

    prng rng("ignored");
    stock = Stock(5, 9, rng);
    CHECK(stock.id() == 5);
    CHECK(stock.bank_level() == 9);
}

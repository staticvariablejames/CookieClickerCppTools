#include "stock.h"
#include "catch2/catch.hpp"

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
        "Game.Objects['Bank'].minigame.goodsById[0].val=10.000000;"
        "Game.Objects['Bank'].minigame.goodsById[0].d=0.000000;"
        "Game.Objects['Bank'].minigame.goodsById[0].mode=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].dur=10;"
    );
    CHECK(stock.javascriptString(true) ==
        "Game.Objects['Bank'].minigame.goodsById[0].val=10.000000;"
        "Game.Objects['Bank'].minigame.goodsById[0].d=0.000000;"
        "Game.Objects['Bank'].minigame.goodsById[0].mode=0;"
        "Game.Objects['Bank'].minigame.goodsById[0].dur=10;"
        "Game.Objects['Bank'].level=1;"
    );

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

    CHECK(stock.javascriptString() ==
        "Game.Objects['Bank'].minigame.goodsById[3].val=32.000000;"
        "Game.Objects['Bank'].minigame.goodsById[3].d=-1.500000;"
        "Game.Objects['Bank'].minigame.goodsById[3].mode=3;"
        "Game.Objects['Bank'].minigame.goodsById[3].dur=550;"
    );
    CHECK(stock.javascriptString(true) ==
        "Game.Objects['Bank'].minigame.goodsById[3].val=32.000000;"
        "Game.Objects['Bank'].minigame.goodsById[3].d=-1.500000;"
        "Game.Objects['Bank'].minigame.goodsById[3].mode=3;"
        "Game.Objects['Bank'].minigame.goodsById[3].dur=550;"
        "Game.Objects['Bank'].level=5;"
    );
}

TEST_CASE("Stock::tick behaves as intended", "[stock]") {
    Stock stock;
    prng rng("test");

    stock.tick(rng);
    CHECK(stock.value() == Approx(10.026040165045062));
    CHECK(stock.delta() == Approx(0.065083024300872));
    CHECK(stock.mode() == StockMode::Stable);
    CHECK(stock.duration() == 9);

    rng = prng("test2");
    stock.tick(rng);
    CHECK(stock.value() == Approx(10.07204888866176));
    CHECK(stock.delta() == Approx(0.03826339941499993));
    CHECK(stock.mode() == StockMode::Stable);
    CHECK(stock.duration() == 8);

    stock.duration(1);
    rng = prng("test3");
    stock.tick(rng);
    CHECK(stock.value() == Approx(9.887772875938166));
    CHECK(stock.delta() == Approx(-0.019272334437613416));
    CHECK(stock.mode() == StockMode::SlowFall);
    CHECK(stock.duration() == 217);
}

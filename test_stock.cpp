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

TEST_CASE("Stock::tick changes mode properly", "[stock]") {
    // I randomly typed those strings on the console...
    Stock stock;
    stock.duration(1);
    SECTION("Staying in the same mode works") {
        prng rng(GENERATE("asentuh", "110123"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::Stable);
    }
    SECTION("Changing to StockMode::SlowRise works") {
        prng rng(GENERATE("aoeu", "asdf"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::SlowRise);
    }
    SECTION("Changing to StockMode::SlowFall works") {
        prng rng(GENERATE("yroaoi", "oeroaoi"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::SlowFall);
    }
    SECTION("Changing to StockMode::FastRise works") {
        prng rng(GENERATE("oeu", "o359rq4"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::FastRise);
    }
    SECTION("Changing to StockMode::FastFall works") {
        prng rng(GENERATE("abc", "o359rqk"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::FastFall);
    }
    SECTION("Changing to StockMode::Chaotic works") {
        prng rng(GENERATE("eu", "adf"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::Chaotic);
    }
}

TEST_CASE("FastRise may switch to FastFall mid-duration", "[stock]") {
    Stock stock;
    stock.mode(StockMode::FastRise);
    SECTION("Sometimes switch") {
        prng rng(GENERATE("728", "729", "799"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::FastFall);
        CHECK(stock.duration() == 9);
    }
    SECTION("But not most times") {
        prng rng(GENERATE("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"));
        stock.tick(rng);
        CHECK(stock.mode() == StockMode::FastRise);
    }
}

TEST_CASE("Delta and value change correctly in all modes", "[stock]") {
    Stock stock;
    stock.value(15.0);
    prng rng("test");

    SECTION("Proper behavior under SlowRise") {
        stock.mode(StockMode::SlowRise);
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.946040165045062));
        CHECK(stock.delta() == Approx(0.085083024300872));

        rng = prng("test2");
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.935380110004635));
        CHECK(stock.delta() == Approx(0.07999462075787377));

        rng = prng("test3");
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.715396584607047));
        CHECK(stock.delta() == Approx(0.04228677731525032));
    }

    SECTION("Proper behavior under SlowFall") {
        stock.mode(StockMode::SlowFall);
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.86881990961346));
        CHECK(stock.delta() == Approx(0.007862768869269461));

        rng = prng("test2");
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.728538588330533));
        CHECK(stock.delta() == Approx(-0.051171050593257564));

        rng = prng("test3");
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.39587007299299));
        CHECK(stock.delta() == Approx(-0.07453504305818688));
    }

    SECTION("Proper behavior under FastRise") {
        stock.mode(StockMode::FastRise);
        stock.tick(rng);
        CHECK(stock.value() == Approx(15.576546860686053));
        CHECK(stock.delta() == Approx(0.09631027952250534));

        rng = prng("test2");
        stock.tick(rng);
        CHECK(stock.value() == Approx(15.886085716427985));
        CHECK(stock.delta() == Approx(0.13639925597461477));

        rng = prng("test3");
        stock.tick(rng);
        CHECK(stock.value() == Approx(11.622525615245264));
        CHECK(stock.delta() == Approx(0.1454186649303845));
    }

    SECTION("Proper behavior under FastFall") {
        stock.mode(StockMode::FastFall);
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.556196104744716));
        CHECK(stock.delta() == Approx(-0.13535048677230224));

        rng = prng("test2");
        stock.tick(rng);
        CHECK(stock.value() == Approx(13.679327678090262));
        CHECK(stock.delta() == Approx(-0.2593448675118389));

        rng = prng("test3");
        stock.tick(rng);
        CHECK(stock.value() == Approx(12.924897891880047));
        CHECK(stock.delta() == Approx(-0.211043413376313));
    }

    SECTION("Proper behavior under Chaotic") {
        stock.mode(StockMode::Chaotic);
        stock.tick(rng);
        CHECK(stock.value() == Approx(14.946031046194994));
        CHECK(stock.delta() == Approx(0.44950271767441685));

        rng = prng("test2");
        stock.tick(rng);
        CHECK(stock.value() == Approx(19.458920755587062));
        CHECK(stock.delta() == Approx(0.45683467881633927));

        rng = prng("test3");
        stock.tick(rng);
        CHECK(stock.value() == Approx(15.37418775813127));
        CHECK(stock.delta() == Approx(0.2657561468906777));
    }
}

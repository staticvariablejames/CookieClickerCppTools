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

TEST_CASE("FastRise and FastFall show bias towards Chaotic", "[stock]") {
    int count[] = {0, 0, 0, 0, 0, 0};

    SECTION("FastRise show bias", "[stock]") {
        for(auto i = 0; i < 1000; i++) {
            Stock stock;
            stock.duration(1);
            stock.mode(StockMode::FastRise);
            prng rng(std::to_string(i));
            stock.tick(rng);
            count[static_cast<int>(stock.mode())]++;
        }
        CHECK(count[0] == 32);
        CHECK(count[1] == 69);
        CHECK(count[2] == 78);
        CHECK(count[3] == 45);
        CHECK(count[4] == 36);
        CHECK(count[5] == 740);
    }

    SECTION("FastFall show bias", "[stock]") {
        for(auto i = 0; i < 1000; i++) {
            Stock stock;
            stock.duration(1);
            stock.mode(StockMode::FastFall);
            prng rng(std::to_string(i));
            stock.tick(rng);
            count[static_cast<int>(stock.mode())]++;
        }
        CHECK(count[0] == 40);
        CHECK(count[1] == 84);
        CHECK(count[2] == 72);
        CHECK(count[3] == 23);
        CHECK(count[4] == 34);
        CHECK(count[5] == 747);
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

TEST_CASE("FastRise and FastFall special behavior", "[stock]") {
    Stock stock;
    stock.value(25);
    stock.delta(1);
    SECTION("FastRise special behavior kicks in") {
        stock.mode(StockMode::FastRise);
        prng rng("test32");
        stock.tick(rng);
        CHECK(stock.value() == Approx(21.04406950902571));
        CHECK(stock.delta() == Approx(1.0916515006483285));

        rng = prng("test19");
        stock.tick(rng);
        CHECK(stock.value() == Approx(16.329421130036295));
        CHECK(stock.delta() == Approx(1.151595180254644));
    }
    SECTION("FastFall special behavior kicks in") {
        stock.mode(StockMode::FastFall);
        prng rng("test32");
        stock.tick(rng);
        CHECK(stock.value() == Approx(23.454135022493432));
        CHECK(stock.delta() == Approx(1.120729409577468));

        rng = prng("test19");
        stock.tick(rng);
        CHECK(stock.value() == Approx(21.196426179939394));
        CHECK(stock.delta() == Approx(1.0959183958178986));
    }
}

TEST_CASE("Large values are affected by the soft cap", "[stock]") {
    Stock stock;
    stock.value(125);
    stock.delta(15);
    prng rng("test4");

    SECTION("Affected with banks level 1") {
        stock.tick(rng);
        CHECK(stock.value() == Approx(135.20450316372376));
        CHECK(stock.delta() == Approx(12.472506641710098));
    }

    SECTION("Still affected with banks level 8") {
        stock.bank_level(8);
        stock.tick(rng);
        CHECK(stock.value() == Approx(135.34450316372374));
        CHECK(stock.delta() == Approx(12.472506641710098));
    }

    SECTION("But not affected with banks level 9") {
        stock.bank_level(9);
        stock.tick(rng);
        CHECK(stock.value() == Approx(136.7503372350249));
        CHECK(stock.delta() == Approx(13.85834071301122));
    }
}

TEST_CASE("Small values are affected by the $1 and $5 thresholds", "[stock]") {
    Stock stock;
    stock.value(1.2);
    stock.delta(-5);

    prng rng("test4");
    stock.tick(rng);
    CHECK(stock.value() == 1.0); // Exact equality here
    CHECK(stock.delta() == Approx(-4.343076322639338));

    rng = prng("test3");
    stock.tick(rng);
    CHECK(stock.value() == 1.0);
    CHECK(stock.delta() == Approx(-3.8538430438951514));

    rng = prng("test2");
    stock.tick(rng);
    CHECK(stock.value() == Approx(1.3076195775374821));
    CHECK(stock.delta() == Approx(-3.3943756238062535));

}

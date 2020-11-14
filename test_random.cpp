#include "catch2/catch.hpp"

#include "random.h"

#include <stdio.h>

using namespace CCCPP;

static_assert(std::uniform_random_bit_generator<rc4_engine>);

TEST_CASE("The RC4 random engine works properly", "[random]") {
    rc4_engine e("test");
    REQUIRE(e() == 223);
    REQUIRE(e() == 72);
    REQUIRE(e() == 170);
    REQUIRE(e() == 166);
    REQUIRE(e() == 70);
    // A million calls later
    for(auto i = 0; i < 1000*1000; i++) {
        (void) e();
    }
    REQUIRE(e() == 31);
    REQUIRE(e() == 239);
    REQUIRE(e() == 17);
    REQUIRE(e() == 186);
    REQUIRE(e() == 15);
}

TEST_CASE("The uniform_random function works properly", "[random]") {
    rc4_engine e("hello.");
    double value = generate_canonical(e);
    REQUIRE(value == Approx(0.9282578795792454));
    CHECK(value == 0.92825787957924543736254463510704226791858673095703125);

    value = generate_canonical(e);
    REQUIRE(value == Approx(0.3752569768646784));
    CHECK(value == 0.37525697686467840430424303121981211006641387939453125);

    /* Reasoning:
     * The approximate values are good enough for most cases,
     * so I used REQUIRE there;
     * it is nice to check for the exact value,
     * but since floating point shenanigans might happen,
     * I am only writing those as "CHECK",
     * and maybe those checks should be removed once this application is validated.
     */
}

#include "catch2/catch.hpp"

#include "random.h"

#include <stdio.h>

using namespace CCCPP;

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

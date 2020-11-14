#include "catch2/catch.hpp"
#include "random_utils.h"

#include <vector>

#include "random.h"

using namespace CCCPP;

TEST_CASE("CCCPP::choose behaves properly", "[random][util]") {
    prng rng("test");
    REQUIRE(choose({0, 1, 2, 3}, rng) == 3);
    REQUIRE(choose({'a', 'b', 'c', 'd', 'e'}, rng) == 'c');
    REQUIRE(choose({1.5, 2.5, 3.5}, rng) == 3.5);
}

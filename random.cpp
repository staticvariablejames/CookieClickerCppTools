#include "random.h"

namespace CCCPP {

rc4_engine::rc4_engine(const std::string& key):
    i(0), j(0)
{
    for(auto x = 0; x < rc4_engine::width; x++) {
        S[x] = x;
    }
    auto y = 0;
    for(auto x = 0; x < 256; x++) {
        y = (y + S[x] + key[x % key.length()]) % rc4_engine::width;
        std::swap(S[x], S[y]);
    }

    // Discard the first 256 values
    for(auto x = 0; x < 256; x++) {
        this->operator()();
    }
}

auto rc4_engine::operator()() -> unsigned {
    i = (i+1) % rc4_engine::width;
    j = (j + S[i]) % rc4_engine::width;
    std::swap(S[i], S[j]);
    return S[(S[i] + S[j]) % rc4_engine::width];
}

auto generate_canonical(rc4_engine& e) -> double {
    constexpr auto significance = 4503599627370496.0; // pow(2, 52)

    auto n = 0.0, d = 1.0; // numerator and denominator
    unsigned x = 0; // next bit to be added to the numerator
    while(n < significance) {
        n = (n + x) * rc4_engine::width;
        d *= rc4_engine::width;
        x = e();
    }

    while(n >= 2*significance) {
        n /= 2;
        d /= 2;
        x >>= 1;
    }
    return (n+x)/d;
}

static const auto null_string = std::string{'\0'};
prng::prng(const std::string& key) :
    engine(key == "" ? null_string : key) // Avoid binding temporary to a reference
{}

auto prng::operator()() -> double {
    return generate_canonical(engine);
}

} // namespace CCCPP

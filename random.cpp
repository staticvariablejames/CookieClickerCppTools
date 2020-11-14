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

auto rc4_engine::operator()() -> unsigned char {
    i = (i+1) % rc4_engine::width;
    j = (j + S[i]) % rc4_engine::width;
    std::swap(S[i], S[j]);
    return S[(S[i] + S[j]) % rc4_engine::width];
}

} // namespace CCCPP

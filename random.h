/* random.h
 * RC4-based pseudorandom number generator, and a uniform real distribution over [0, 1).
 *
 * This is a C++ port of David Bau's seedrandom (https://github.com/davidbau/seedrandom).
 * Since the intent was to replicate only the functionality used by Cookie Clicker,
 * only the seeding and random generation was implemented.
 */
#ifndef RANDOM_H
#define RANDOM_H

#include <array>
#include <string>

namespace CCCPP {

class rc4_engine {
public:
    explicit rc4_engine(const std::string& key);

    // std::uniform_random_bit_generator requirements
    using result_t = unsigned char;
    static constexpr result_t min() { return 0; }
    static constexpr result_t max() { return 255; }
    result_t operator()();

    static constexpr unsigned width = 256;
private:
    unsigned i;
    unsigned j;
    std::array<unsigned char, width> S;
};

/* Uses the given engine to generate a random number between 0 (inclusive) and 1 (exclusive).
 *
 * This is David Bau's `prng` function.
 */
double uniform_random_variate(rc4_engine&);

} // namespace CCCPP

#endif // RANDOM_H

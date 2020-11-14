/* random.h
 * RC4-based pseudorandom number generator,
 * a uniform real distribution over [0, 1),
 * and a convenience class that binds both.
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
    // key must not be ""
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
 * This is functionally equivalent to std::generate_canonical,
 * hence the name.
 *
 * This is David Bau's `prng` function.
 */
double generate_canonical(rc4_engine&);

/* Convenience class which calls generate_canonical in an internal rc4_engine.
 *
 * If in the constructor key is "",
 * it is treated as "\0" instead.
 */
class prng {
public:
    explicit prng(const std::string& key);
    double operator()();
private:
    rc4_engine engine;
};

} // namespace CCCPP

#endif // RANDOM_H

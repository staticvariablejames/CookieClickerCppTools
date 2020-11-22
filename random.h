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
#include <random>
#include <string>

namespace CCCPP {

class rc4_engine {
public:
    // key must not be ""
    explicit rc4_engine(const std::string& key);

    // std::uniform_random_bit_generator requirements
    using result_t = unsigned;
    static constexpr auto min() -> result_t { return 0; }
    static constexpr auto max() -> result_t { return 255; }
    auto operator()() -> result_t;

    static constexpr auto width = 256u;
private:
    unsigned i;
    unsigned j;
    std::array<unsigned, width> S;
    /* Aligned access is faster than unaligned access;
     * using unsigned here (instead of, say, unsigned char)
     * gives about 10% better performance.
     */
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
    auto operator()() -> double;
private:
    rc4_engine engine;
};

/* Returns an object that behaves similarly to the prng class above,
 * but using random number generators from the standard library.
 */
template<typename Engine = std::minstd_rand>
auto std_prng(const std::string& key) {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::seed_seq seedseq(key.begin(), key.end());
    Engine engine(seedseq);
    return [=]() mutable {
        return distribution(engine);
    };
}

} // namespace CCCPP

#endif // RANDOM_H

/* random_utils.h
 * A few utilities that use random number generators to perform actions.
 */
#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include<initializer_list>
#include<vector>

namespace CCCPP {

/* Randomly chooses one element of the vector and return it.
 * This is a port of Orteil's choose.
 */
template<typename Vector, typename RNG>
auto choose(const Vector& vec, RNG& rng) {
    auto index = static_cast<int>(rng() * vec.size());
    return vec[index];
}

/* Explicit overload for initializer lists,
 * because the standard explicitly excludes braced lists to argument type deduction.
 * This way choose({1, 2, 3}, rng) works.
 */
template<typename T, typename RNG>
auto choose(const std::initializer_list<T>& list, RNG& rng) {
    return choose<std::vector<T>>(list, rng);
}

} // namespace CCCPP

#endif // RANDOM_UTILS_H

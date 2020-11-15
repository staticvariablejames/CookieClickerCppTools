/* stock.h
 * Encapsulates all data relative to a single stock market good,
 * plus the fluctuation algorithm.
 *
 * Note that this class only has the data necessary to compute the fluctuations;
 * no stock amounts or profits or stockbrokers are tracked.
 */
#ifndef STOCK_H
#define STOCK_H

#include "random.h"

namespace CCCPP {

enum class StockMode {
    Stable,
    SlowRise,
    SlowFall,
    FastRise,
    FastFall,
    Chaotic,
};

class Stock {
private:
    double _value = 10.0;
    double _delta = 0.0;
    StockMode _mode = StockMode::Stable;
    int _duration = 10;
    int _id = 0;
    int _bank_level = 1;
    /* These default values are here to make the class default-constructible
     * and have the default-constructed value usable.
     */

public:
    // Setters and getters
    auto value() const { return _value; }
    void value(double value) { _value = value; }

    auto delta() const { return _delta; }
    void delta(double delta) { _delta = delta; }

    auto mode() const { return _mode; }
    void mode(StockMode mode) { _mode = mode; }

    auto duration() const { return _duration; }
    void duration(int duration) { _duration = duration; }

    auto id() const { return _id; }
    void id(int id) { _id = id; }

    auto bank_level() const { return _bank_level; }
    void bank_level(int bank_level) { _bank_level = bank_level; }

    Stock() = default;
};

}

#endif // STOCK_H

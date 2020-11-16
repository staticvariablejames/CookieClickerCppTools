/* stock.h
 * Encapsulates all data relative to a single stock market good,
 * plus the fluctuation algorithm.
 *
 * Note that this class only has the data necessary to compute the fluctuations;
 * no stock amounts or profits or stockbrokers are tracked.
 */
#ifndef STOCK_H
#define STOCK_H

#include <string>

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

    auto restingValue() const {
        return _bank_level + 10.0*_id + 9;
    }

    auto softCap() const {
        return 97.0 + 3*_bank_level;
    }

    /* Returns a string of a sequence of Javascript asignments
     * that changes the state of the corresponding Cookie Clicker stock
     * to match the state of this object.
     *
     * If include_bank is true, it also includes a command to change the bank level.
     *
     * This is mostly useful for testing.
     */
    auto javascriptString(bool include_bank=false) const {
        using namespace std::string_literals;
        using std::to_string;
        auto goodStr = "Game.Objects['Bank'].minigame.goodsById["s + to_string(_id) + "]"s;
        auto str = ""s;
        str += goodStr + ".val=" + to_string(_value) + ';';
        str += goodStr + ".d=" + to_string(_delta) + ';';
        str += goodStr + ".mode=" + to_string(static_cast<int>(_mode)) + ';';
        str += goodStr + ".dur=" + to_string(_duration) + ';';
        if(include_bank) {
            str += "Game.Objects['Bank'].level=" + to_string(_bank_level) + ';';
        }
        return str;
    }
};

}

#endif // STOCK_H

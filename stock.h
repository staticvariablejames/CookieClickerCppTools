/* stock.h
 * Encapsulates all data relative to a single stock market good,
 * plus the fluctuation algorithm.
 *
 * Note that this class only has the data necessary to compute the fluctuations;
 * no stock amounts or profits or stockbrokers are tracked.
 */
#ifndef STOCK_H
#define STOCK_H

#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>

#include "random_utils.h"

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
    int _id = 0;
    int _bank_level = 1;
    double _value = 10.0;
    double _delta = 0.0;
    StockMode _mode = StockMode::Stable;
    int _duration = 10;
    /* These default values are here to make the class default-constructible
     * and have the default-constructed value usable.
     */

public:
    /*
     * Number of goods defined by Cookie Clicker (one for each building except cursors and grandmas).
     */
    static constexpr unsigned count = 18;

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

        std::stringstream ss;
        ss << goodStr << ".val=" << std::setprecision(17) << _value << ';';
        ss << goodStr << ".d=" << std::setprecision(17) << _delta << ';';
        ss << goodStr << ".mode=" << static_cast<int>(_mode) << ';';
        ss << goodStr << ".dur=" << _duration << ';';
        if(include_bank) {
            ss << "Game.Objects['Bank'].level=" << _bank_level << ';';
        }
        return ss.str();
    }

    // Weighted random mode chooser.
    template<typename RNG>
    static auto chooseMode(RNG& rng) {
        return choose({
                StockMode::Stable, StockMode::SlowRise, StockMode::SlowRise, StockMode::SlowFall,
                StockMode::SlowFall, StockMode::FastRise, StockMode::FastFall, StockMode::Chaotic,
            },
            rng);
    }

    /* Constructs a stock with the given id and bank level,
     * using the same initialization procedure done by the game.
     */
    template<typename RNG>
    Stock(int id, int bank_level, RNG& rng):
        _id{id},
        _bank_level{bank_level},
        _value{restingValue()},
        _delta{0.2*rng() - 0.1},
        _mode{chooseMode(rng)},
        _duration{static_cast<int>(10 + rng()*690)}
    {
        for(auto i = 0; i < 15; i++)
            tick(rng, 0.0);
    }

    struct global_parameters {
        double D;
        double P;
    };

    template<typename RNG>
    static global_parameters generateGlobalParameters(RNG& rng, double dragonBoost) {
        global_parameters glob;
        glob.D = 0.0;
        glob.P = rng();
        if(rng() < 0.1 + 0.1*dragonBoost) glob.D = (rng() - 0.5)*2;
        return glob;
    }

    template<typename RNG>
    void singleStockTick(RNG& rng, const global_parameters& glob, double dragonBoost) {
        _delta *= 0.97 + 0.01*dragonBoost;
        switch(_mode) {
            case StockMode::Stable:     _delta *= 0.95; _delta += 0.05*(rng()-0.5); break;
            case StockMode::SlowRise:   _delta *= 0.99; _delta += 0.05*(rng()-0.1); break;
            case StockMode::SlowFall:   _delta *= 0.99; _delta -= 0.05*(rng()-0.1); break;
            case StockMode::FastRise:   _delta += 0.15*(rng()-0.1); _value += 5*rng(); break;
            case StockMode::FastFall:   _delta -= 0.15*(rng()-0.1); _value -= 5*rng(); break;
            case StockMode::Chaotic:    _delta += 0.3*(rng()-0.5); break;
        }

        _value += 0.01*(restingValue()-_value);
        if(glob.D != 0.0 && rng() < glob.P) {
            _value -= glob.D * (1 + 7 * _delta * std::pow(rng(), 3));
            _value -= glob.D * (1 + 7 * std::pow(rng(), 3));
            _delta += glob.D * (1 + 4 * rng());
            _duration = 0;
        }

        _value += 3*std::pow(2*(rng()-0.5), 11);
        _delta += 0.1*(rng()-0.5);
        if(rng() < 0.15) _value += 3*(rng()-0.5);
        if(rng() < 0.03) _value += (10 + 10*dragonBoost) * (rng()-0.5);
        if(rng() < 0.1) _delta += (0.3 + 0.2*dragonBoost) * (rng()-0.5);


        if(_mode == StockMode::Chaotic) {
            if(rng() < 0.5) _value += 10*(rng()-0.5);
            if(rng() < 0.2) _delta = (2 + 6*dragonBoost) * (rng()-0.5);
        }
        if(_mode == StockMode::FastRise && rng() < 0.3) {
            _delta += 0.1*(rng()-0.5);
            _value += 10*(rng()-0.7);
        }
        if(_mode == StockMode::FastRise && rng() < 0.03) {
            _mode = StockMode::FastFall;
        }
        if(_mode == StockMode::FastFall && rng() < 0.3) {
            _delta += 0.1*(rng()-0.5);
            _value += 10*(rng()-0.3);
        }

        if(_value > softCap() && _delta > 0) {
            _delta *= 0.9;
        }

        _value += _delta;

        if(_value < 5) _value += 0.5*(5-_value);
        if(_value < 5 && _delta < 0) _delta *= 0.95;
        if(_value < 1) _value = 1.0;

        _duration--;
        if(_duration <= 0) {
            _duration = static_cast<int>(10 + (690 - 200*dragonBoost) * rng());
            if(rng() < dragonBoost && rng() < 0.5) {
                _mode = StockMode::Chaotic;
            } else if(rng() < 0.7 && (_mode == StockMode::FastRise || _mode == StockMode::FastFall)) {
                _mode = StockMode::Chaotic;
            } else {
                _mode = chooseMode(rng);
            }
        }
    }

    template<typename RNG>
    void tick(RNG& rng, double dragonBoost) {
        auto glob = Stock::generateGlobalParameters(rng, dragonBoost);
        this->singleStockTick(rng, glob, dragonBoost);
    }
};

}

#endif // STOCK_H

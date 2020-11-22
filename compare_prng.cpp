/* compare_prng.cpp
 * Small non-interactive program meant to test
 * (using the Kolmogorov-Smirnov test)
 * whether I can use a standard library in Stock::tick or not.
 */
#include <iostream>

#include "random.h"
#include "stock.h"
#include "stock_analysis.h"

using namespace CCCPP;

double kstest(histogram h1, histogram h2) {
    auto sum1 = 0;
    auto sum2 = 0;
    auto maxdiff = 0;
    h1.data.resize(std::max(h1.data.size(), h2.data.size()));
    h2.data.resize(std::max(h1.data.size(), h2.data.size()));
    auto it1 = h1.data.begin();
    auto it2 = h2.data.begin();
    while(it1 != h1.data.end()) {
        sum1 += *it1++;
        sum2 += *it2++;
        maxdiff = std::max(maxdiff, std::max(sum1-sum2, sum2-sum1));
    }
    return static_cast<double>(maxdiff)/sum1;
}


int main() {
    histogram h1 = [](){
        auto rng = std_prng<std::mt19937>("CRM1");
        Stock stock(10, 1, rng);
        (void) generateStockHistogram(stock, rng, histogram::year_of_ticks);
        return generateStockHistogram(stock, rng, 100*histogram::year_of_ticks);
    }();
    histogram h2 = [](){
        auto rng = std_prng("CRM1");
        Stock stock(10, 1, rng);
        (void) generateStockHistogram(stock, rng, histogram::year_of_ticks);
        return generateStockHistogram(stock, rng, 100*histogram::year_of_ticks);
    }();
    std::cout << "KS-test gives " << kstest(h1, h2) << '\n';
}

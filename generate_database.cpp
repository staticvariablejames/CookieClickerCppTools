#include <cstdlib>
#include <iostream>

#include "random.h"
#include "stock.h"
#include "stock_histogram.h"

int main(int argc, char ** argv) {
    int bank_level = 0;
    if(argc >= 2) bank_level = std::atoi(argv[1]);
    if(bank_level <= 0) {
        std::cerr << "Usage: " << argv[0] << " bank_level\n";
        return 1;
    }

    std::string names[] = {
        "CRL",
        "CHC",
        "BTR",
        "SUG",
        "NUT",
        "SLT",
        "VNL",
        "EGG",
        "CNM",
        "CRM",
        "JAM",
        "WCH",
        "HNY",
        "CKI",
        "RCP",
        "SBD",
        "PBL",
        "YOU",
    };

    std::cout << "InsugarTrading.data[" << bank_level << "] = {0.0: [], 0.1: [], 1.0: [], 1.1: []};\n";
    for(auto i = 0; i < CCCPP::Stock::count; i++) {
        std::clog << "Generating " << names[i] << std::endl;
        // Just some reproducible seed
        auto rng = CCCPP::std_prng<std::mt19937_64>(names[i] + std::to_string(bank_level));
        CCCPP::Stock stock(i, bank_level, rng);
        // Discard the first year
        (void) CCCPP::generateStockHistogram(stock, rng, CCCPP::histogram_set::year_of_ticks);
        auto h = CCCPP::generateStockHistogram(stock, rng, 1000*CCCPP::histogram_set::year_of_ticks);
        std::cout << h;
    }
}

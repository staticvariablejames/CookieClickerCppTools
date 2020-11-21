#include <cstdlib>
#include <iostream>

#include "random.h"
#include "stock.h"
#include "stock_analysis.h"

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
    };

    std::cout << "InsugarTrading.data[" << bank_level << "] = [];\n";
    for(auto i = 0; i < 16; i++) {
        std::clog << "Generating " << names[i] << std::endl;
        CCCPP::prng rng(names[i] + std::to_string(bank_level));
        CCCPP::Stock stock(i, bank_level, rng);
        // Discard the first year
        (void) CCCPP::generateStockHistogram(stock, rng, CCCPP::histogram::year_of_ticks);
        auto h = CCCPP::generateStockHistogram(stock, rng, 1000*CCCPP::histogram::year_of_ticks);
        std::cout << h << '\n';
    }
}

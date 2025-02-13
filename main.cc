#include <cstdint>
#include <vector>
#include <chrono>
#include <numeric>
#include <iostream>

#include "proj/pr_lapackpp.hh"
#include "proj/pr_algorithm.hh"


int main() {
    using T = double;
    int64_t m = 7;
    int64_t n = 1;
    std::vector<T> A(m*n, 2.0);
    std::vector<T> R(n*n, 0.0);
    cholqr_offload_nocuda(m, n, A.data(), m, R.data(), n);

    std::cout << "A:\n";
    for (auto ai : A) {
        std::cout << ai << std::endl;
    }
    for (auto r : R) {
        std::cout << r << std::endl;
    }
    return 0;
}

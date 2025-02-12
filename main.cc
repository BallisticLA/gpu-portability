#include <cstdint>
#include <vector>
#include <chrono>
#include <numeric>

#include "proj/pr_lapackpp.hh"
#include "proj/pr_algorithm.hh"


int main() {
    using T = double;
    int64_t m = 4;
    int64_t n = 1;
    std::vector<T> A(m*n, 0.0);
    std::vector<T> R(n*n, 0.0);
    cholqr_offload_nocuda(m, n, A.data(), m, R.data(), n);
    return 0;
}

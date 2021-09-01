#include <vector>

#include "pmutils.hpp"

int main() {
    auto x = std::vector<int>{10, 0};
    auto y = std::vector<int>{100, 0};
    auto z = std::vector<int>{10'000'000, 0};

    void* ptr = malloc(1024);
    for (auto i = 0; i < 100; ++i) {
        ptr = malloc(512ul << 20);
        pmutils::do_not_optimize(ptr);
    }

    pmutils::do_not_optimize(x);
    pmutils::do_not_optimize(y);
    pmutils::do_not_optimize(z);

    return 0;
}
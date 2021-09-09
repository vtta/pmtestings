#include <libpmem.h>
#include <numa.h>
#include <oneapi/tbb.h>
#include <spdlog/spdlog.h>

#include <execution>

#include "pmutils.hpp"

constexpr auto POOL_SIZE = 8ul << 30;

int main() {
    // pmutils::cpubind(12);
    void *drampool[] = {
        numa_alloc_onnode(POOL_SIZE, 0),
        numa_alloc_onnode(POOL_SIZE, 1),
        numa_alloc_onnode(POOL_SIZE, 2),
        numa_alloc_onnode(POOL_SIZE, 3),
    };
    for (auto i = 0ul; i < 4; ++i) {
        auto dram = (char *)drampool[i];
        tbb::parallel_for(tbb::blocked_range{0ul, POOL_SIZE}, [&](auto &r) {
            auto begin = dram + r.begin();
            pmutils::prefault(begin, r.size(), 4ul << 10);
        });
    }
    void *pmempool[] = {
        pmutils::open("/mnt/pmem0/jlhu/random-testing-0", POOL_SIZE),
        pmutils::open("/mnt/pmem1/jlhu/random-testing-0", POOL_SIZE),
        pmutils::open("/mnt/pmem2/jlhu/random-testing-0", POOL_SIZE),
        pmutils::open("/mnt/pmem3/jlhu/random-testing-0", POOL_SIZE),
    };
    for (auto i = 0ul; i < 4; ++i) {
        auto pmem = (char *)pmempool[i];
        tbb::parallel_for(tbb::blocked_range{0ul, POOL_SIZE}, [&](auto &r) {
            auto begin = pmem + r.begin();
            pmem_memset_persist(begin, 'x', r.size());
        });
    }

    for (auto node = 0ul; node < 4; ++node) {
        auto dram = drampool[node];
        auto actual_node = pmutils::numa_node_of(dram);
        fmt::print("dram pool {} ptr {} node {} expected {}\n", node, dram,
                   actual_node, node);
    }

    for (auto i = 0ul; i < 4; ++i) {
        auto dram = (char *)drampool[i];
        for (auto j = 0ul; j < 4; ++j) {
            auto pmem = (char *)pmempool[j];
            // single thread
            // auto t = pmutils::time([&] { memcpy(dram, pmem, POOL_SIZE); });
            // multi thread
            auto t = pmutils::time([&] {
                tbb::parallel_for(
                    tbb::blocked_range{0ul, POOL_SIZE}, [&](auto &r) {
                        memcpy((char *)dram + r.begin(), (char *)pmem + r.begin(), r.size());
                    });
            });
            spdlog::info("DRAM {} <- PMEM {} speed {:.4f} GiB/s", i, j,
                         (double)POOL_SIZE / t.count());
        }
    }

    std::for_each(drampool, drampool + 4,
                  std::bind(numa_free, std::placeholders::_1, POOL_SIZE));
    return 0;
}
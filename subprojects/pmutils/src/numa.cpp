#include "pmutils/numa.hpp"

#include <numa.h>
#include <numaif.h>
#include <pthread.h>
#include <sched.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/spdlog.h>

namespace pmutils {

void cpubind(int idx) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(idx, &set);
    int err = pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
    if (err) {
        spdlog::error("thread {} pthread_setaffinity_np {}", pthread_self(),
                      err);
        exit(err);
    }
    spdlog::info("thread {} running on core {} node {}", pthread_self(),
                 sched_getcpu(), numa_node_of_cpu(sched_getcpu()));
}

int numa_node_of(void *ptr) {
    int node = -1, err = 0;
    if (0 != (err = get_mempolicy(&node, nullptr, 0, ptr,
                                  MPOL_F_NODE | MPOL_F_ADDR))) {
        spdlog::error("get_mempolicy returned {}", err);
        exit(err);
    }
    return node;
}

void prefault(void *begin, size_t len, size_t granularity) {
    unsigned char *base = (unsigned char *)begin;
    for (size_t i = 0; i < len; i += granularity) {
        auto reg = base[i];
        // do_not_optimize(reg); // no need
        base[i] = reg;
    }
    return;
}

}  // namespace pmutils
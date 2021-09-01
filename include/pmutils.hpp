#ifdef DISABLE_LOGGING
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif

#include <libpmem.h>
#include <numa.h>
#include <numaif.h>
#include <pthread.h>
#include <sched.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace fmt {

// template <typename S, typename... Args>
// requires internal::is_string<S>::value inline void println(const S
// &format_str,
//                                                            Args
//                                                            &&...args) {
//     vprint(to_string_view(format_str),
//            internal::make_args_checked<Args...>(format_str, args...));
//     print("\n");
// }

}  // namespace fmt

namespace pmutils {

template <typename T>
void do_not_optimize(T const &val) {
    asm volatile("" : : "m"(val) : "memory");
}

template <typename Callable, typename... Args>
requires std::logical_not<
    std::is_void<std::invoke_result_t<Callable, Args...>>>::value
    std::tuple<std::chrono::nanoseconds,
               std::invoke_result_t<Callable, Args...>>
    time(Callable &&f, Args &&...args) {
    using clock = std::chrono::high_resolution_clock;
    auto begin = clock::now();
    auto ret = f(std::forward<Args>(args)...);
    auto end = clock::now();
    return {end - begin, ret};
}

template <typename Callable, typename... Args>
requires std::is_void<std::invoke_result_t<Callable, Args...>>::value
    std::chrono::nanoseconds
    time(Callable &&f, Args &&...args) {
    using clock = std::chrono::high_resolution_clock;
    auto begin = clock::now();
    f(std::forward<Args>(args)...);
    auto end = clock::now();
    return end - begin;
}

inline void cpubind(int idx) {
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

inline int numa_node_of(void *ptr) {
    int node = -1, err = 0;
    if (0 != (err = get_mempolicy(&node, nullptr, 0, ptr,
                                  MPOL_F_NODE | MPOL_F_ADDR))) {
        spdlog::error("get_mempolicy returned {}", err);
        exit(err);
    }
    return node;
}

inline void prefault(void *begin, size_t len, size_t granularity = 2ul << 20) {
    unsigned char *base = (unsigned char *)begin;
    for (size_t i = 0; i < len; i += granularity) {
        auto reg = base[i];
        // do_not_optimize(reg); // no need
        base[i] = reg;
    }
    return;
}

inline void *open(char const *path, size_t len) {
    void *ret = nullptr;

    size_t mapped_len;
    int is_pmem;
    if (std::filesystem::exists(path)) {
        fmt::print("PM pool {} exists\n", path);
        if (nullptr == (ret = pmem_map_file(path, 0, PMEM_FILE_EXCL, 0666,
                                            &mapped_len, &is_pmem))) {
            throw std::runtime_error(pmem_errormsg());
        }
    } else {
        fmt::print("PM pool {} created\n", path);
        if (nullptr ==
            (ret = pmem_map_file(path, len, PMEM_FILE_CREATE | PMEM_FILE_EXCL,
                                 0666, &mapped_len, &is_pmem))) {
            throw std::runtime_error(pmem_errormsg());
        }
        // only need to do this once
        prefault(ret, len);
    }

    return ret;
}

}  // namespace pmutils
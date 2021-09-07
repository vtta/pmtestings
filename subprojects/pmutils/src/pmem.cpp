#include "pmutils/pmem.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

#include "libpmem.h"
#include "pmutils/numa.hpp"

namespace pmutils {

void *open(char const *path, size_t len) {
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
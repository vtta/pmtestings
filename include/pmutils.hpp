#pragma once
#ifdef DISABLE_LOGGING
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif

#include "pmutils/load.hpp"
#include "pmutils/numa.hpp"
#include "pmutils/pmem.hpp"
#include "pmutils/store.hpp"
#include "pmutils/time.hpp"

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

}  // namespace pmutils
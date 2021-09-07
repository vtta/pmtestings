#pragma once
#include <type_traits>
namespace pmutils {

template <typename Ptr>
requires(1ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("movb %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(2ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("movw %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(4ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("movl %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(8ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("movq %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(16ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("vmovdqa %0, (%1)" : : "v"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(32ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("vmovdqa32 %0, (%1)" : : "v"(val), "r"(ptr) : "memory");
}
template <typename Ptr>
requires(64ul == sizeof(std::remove_pointer_t<Ptr>)) void inline store(
    Ptr ptr, std::remove_pointer_t<Ptr> val) {
    __asm__ volatile("vmovdqa64 %0, (%1)" : : "v"(val), "r"(ptr) : "memory");
}

// template <typename Ptr>
// requires std::is_pointer_v<Ptr>
// void inline store(Ptr ptr, std::remove_pointer_t<Ptr> val) {
//     auto constexpr width = sizeof(std::remove_pointer_t<Ptr>);
//     if constexpr (width == 1ul) {
//         __asm__ volatile("movb %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
//     } else if (width == 2ul) {
//         __asm__ volatile("movw %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
//     } else if (width == 4ul) {
//         __asm__ volatile("movl %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
//     } else if (width == 8ul) {
//         __asm__ volatile("movq %0, (%1)" : : "r"(val), "r"(ptr) : "memory");
//     } else if (width == 16ul) {
//         __asm__ volatile("vmovdqa %0, (%1)" : : "v"(val), "r"(ptr) :
//         "memory");
//     } else if (width == 32ul) {
//         __asm__ volatile("vmovdqa32 %0, (%1)"
//                          :
//                          : "v"(val), "r"(ptr)
//                          : "memory");
//     } else if (width == 64ul) {
//         __asm__ volatile("vmovdqa64 %0, (%1)"
//                          :
//                          : "v"(val), "r"(ptr)
//                          : "memory");
//     } else {
//         // unimplemented
//         static_assert(false);
//     }
// }

}  // namespace pmutils
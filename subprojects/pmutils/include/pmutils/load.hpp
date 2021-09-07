#pragma once
#include <type_traits>
namespace pmutils {

template <typename Ptr>
requires(1ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("movb (%1), %0" : "=r"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(2ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("movw (%1), %0" : "=r"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(4ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("movl (%1), %0" : "=r"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(8ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("movq (%1), %0" : "=r"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(16ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("vmovdqa (%1), %0" : "=v"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(32ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("vmovdqa32  (%1), %0" : "=v"(ret) : "r"(ptr) :);
    return ret;
}
template <typename Ptr>
requires(64ul == sizeof(std::remove_pointer_t<Ptr>)) inline auto load(Ptr ptr) {
    std::remove_pointer_t<Ptr> ret;
    __asm__ volatile("vmovdqa64  (%1), %0" : "=v"(ret) : "r"(ptr) :);
    return ret;
}

}  // namespace pmutils
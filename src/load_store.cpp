#include <immintrin.h>

#include <cassert>
#include <concepts>
#include <iostream>

#include "pmutils.hpp"
using namespace pmutils;

int main() {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    __m128i i128;
    __m256i i256;
    __m512i i512;

    __m128i v128 = {128, 128};
    __m256i v256 = {256, 256, 256, 256};
    __m512i v512 = {512, 512, 512, 512, 512, 512, 512, 512};

    store(&i8, 8);
    store(&i16, 16);
    store(&i32, 32);
    store(&i64, 64);
    store(&i128, v128);
    store(&i256, v256);
    store(&i512, v512);

    assert(load(&i8) == 8);
    assert(load(&i16) == 16);
    assert(load(&i32) == 32);
    assert(load(&i64) == 64);
    assert(_mm_testn_epi64_mask(load(&i128), v128) == 0);
    assert(_mm256_testn_epi64_mask(load(&i256), v256) == 0);
    assert(_mm512_testn_epi64_mask(load(&i512), v512) == 0);

    return 0;
}
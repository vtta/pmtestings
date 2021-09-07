#pragma once
#include <cstdio>
namespace pmutils {

void cpubind(int idx);
int numa_node_of(void *ptr);
void prefault(void *begin, size_t len, size_t granularity = 2ul << 20);

}  // namespace pmutils
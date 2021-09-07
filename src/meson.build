
tbb = dependency('TBB')
libpmem = dependency('libpmem') 
spdlog = dependency('spdlog') 
numa = cxx.find_library('libnuma')

deps = [libpmem, spdlog, numa, tbb]
cpp_args = ['-march=native']

pmem_dram_migration = executable('pmem_dram_migration',
           'pmem_dram_migration.cpp',
           include_directories: include,
           dependencies: deps,
           cpp_args: cpp_args)
test('pmem_dram_migration', pmem_dram_migration)


bigmem = executable('bigmem',
           'bigmem.cpp',
           include_directories: include,
           dependencies: deps,
           cpp_args: cpp_args)
# test('bigmem', bigmem)

load_store = executable('load_store',
           'load_store.cpp',
           include_directories: include,
           dependencies: deps,
           cpp_args: cpp_args)
# test('load_store', load_store)
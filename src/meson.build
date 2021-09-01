
tbb = dependency('TBB')
libpmem = dependency('libpmem') 
spdlog = dependency('spdlog') 
numa = cxx.find_library('libnuma')

deps = [libpmem, spdlog, numa, tbb]
cpp_args = ['-march=native']

main = executable('main',
           'main.cpp',
           include_directories: include,
           dependencies: deps,
           cpp_args: cpp_args)
test('main', main)


bigmem = executable('bigmem',
           'bigmem.cpp',
           include_directories: include,
           dependencies: deps,
           cpp_args: cpp_args)

# test('bigmem', bigmem)

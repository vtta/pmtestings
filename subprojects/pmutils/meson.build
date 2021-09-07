project('pmutils', 'cpp',
  version : '0.0.1',
  default_options : ['buildtype=debugoptimized', 'warning_level=3', 'cpp_std=gnu++2a', 'c_std=gnu17'])
# 'buildtype=debugoptimized'
# 'buildtype=release'

libpmem = dependency('libpmem') 
spdlog = dependency('spdlog') 
numa = dependency('numa')
# numa = meson.get_compiler('cpp').find_library('numa')
deps = [libpmem, spdlog, numa]

include = include_directories('include')
subdir('src')

libpmutils_dep = declare_dependency(
    include_directories: include, 
    link_with: [libpmutils], 
    dependencies: deps)

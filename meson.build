project('random-testing', 'cpp',
  version : '0.0.1',
  default_options : ['buildtype=debugoptimized', 'warning_level=3', 'cpp_std=gnu++2a', 'c_std=gnu17'])
# 'buildtype=debugoptimized'
# 'buildtype=release'
cxx = meson.get_compiler('cpp')

pmutils = subproject('pmutils')
pmutils_dep = pmutils.get_variable('libpmutils_dep')

include = include_directories('include')
subdir('src')

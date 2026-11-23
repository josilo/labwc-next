project('tomlc17', 'c', default_options: ['c_std=c17'])

inc = include_directories('src')
lib = static_library('tomlc17', 'src/tomlc17.c', include_directories: inc)
tomlc17_dep = declare_dependency(link_with: lib, include_directories: inc)

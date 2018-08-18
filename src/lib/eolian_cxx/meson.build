eolian_cxx_header_src= files(
  'Eolian_Cxx.hh'
)

subdir('grammar')

eolian_cxx_lib = disabler()

eolian_cxx = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: [eina, eina_cxx, eolian, eo_cxx]
)

install_headers(eolian_cxx_header_src,
  subdir: dir_package_include
)

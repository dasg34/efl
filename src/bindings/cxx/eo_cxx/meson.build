eo_cxx_header_src = files(
  'eo_concrete.hh',
  'eo_cxx_interop.hh',
  'eo_event.hh',
  'Eo.hh',
  'eo_init.hh',
  'eo_ops.hh',
  'eo_wref.hh',
  'eo_private.hh',
  'efl_object_impl.hh'
)

pub_eo_files = [
  join_paths(meson.current_source_dir(), 'src', 'lib', 'eo', 'efl_object.eo')
]

pub_eo_file_target = []

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eolian_cxx_gen_' + eo_file,
      input : eo_file,
      output : [eo_file + '.hh'],
      install : true,
      install_dir : dir_package_include,
      command : [eolian_cxx_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                                 '-o', join_paths(meson.current_build_dir(), eo_file + '.hh'),
                                 '@INPUT@'])
endforeach

eo_cxx_lib = disabler()

eo_cxx = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: [eina, eo]
)

install_headers(eo_cxx_header_src,
  subdir: dir_package_include
)

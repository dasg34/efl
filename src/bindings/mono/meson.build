add_languages('cs')

mono_sublibs = [
  ['Eina',        true, ], #
  ['Eolian',      true, ], #
  ['Eo',          true, ], #
  ['Ecore',      false, ], #
  ['Efl',         true, ], #
  ['Evas',       false, ], #
  ['Edje',       false, ], #
  ['Eldbus',      true, ], #
  ['Ecore_Evas',  true, ], #
  ['Elementary', false, ]  #
]

blacklisted_files = [
  'efl_canvas_text.eo',
  'efl_canvas_scene3d.eo',
  'evas_canvas3d_camera.eo',
  'evas_canvas3d_light.eo',
  'evas_canvas3d_material.eo',
  'evas_canvas3d_mesh.eo',
  'evas_canvas3d_node.eo',
  'evas_canvas3d_object.eo',
  'evas_canvas3d_primitive.eo',
  'evas_canvas3d_scene.eo',
  'evas_canvas3d_texture.eo',
  'efl_canvas_vg_object.eo',
  'efl_vg.eo',
  'efl_vg_container.eo',
  'efl_vg_gradient.eo',
  'efl_vg_gradient_radial.eo',
  'efl_vg_gradient_linear.eo',
  'efl_vg_root_node.eo',
  'efl_vg_shape.eo.cs',
  'efl_io_buffer.eo',
  'efl_io_positioner.eo',
  'efl_io_queue.eo',
  'efl_io_sizer.eo',
  'efl_io_closer_fd.eo',
  'efl_io_buffered_stream.eo',
  'efl_io_positioner_fd.eo',
  'efl_io_reader_fd.eo',
  'efl_io_writer_fd.eo',
  'efl_io_copier_fd.eo',
  'efl_io_sizer_fd.eo',
  'efl_io_stdin.eo',
  'efl_io_stdout.eo',
  'efl_io_stderr.eo',
  'efl_io_file.eo',
  'efl_io_copier.eo',
  'efl_object_override.eo',
  'elm_web.eo',
  'elm_map.eo',
  'elm_combobox.eo',
  'elm_list.eo',
  'elm_genlist.eo',
  'elm_view_list.eo',
  'elm_genlist_item.eo',
  'elm_gengrid.eo',
  'elm_glview.eo.cs'
]

efl_mono_lib = library('eflcustomexportsmono',
    join_paths('..', '..', 'lib', 'efl_mono', 'efl_custom_exports_mono.c'),
    install : true,
    dependencies : [eo, eina]
)

mono_generator_target = []
mono_files = []
foreach lib : mono_sublibs
  package_name = lib[0].to_lower()
  eo_file_subdirs = get_variable(package_name + '_eo_subdirs')
  file_location = join_paths('..', '..', 'lib', package_name)

  if (package_name != 'eldbus')
    foreach eo_file_subdir : eo_file_subdirs
      if eo_file_subdir != ''
        mono_pub_eo_files = get_variable(package_name + '_' + eo_file_subdir +'_eo_files') + get_variable(package_name + '_' + eo_file_subdir + '_eot_files')
      else
        mono_pub_eo_files = get_variable(package_name +'_eo_files') + get_variable(package_name + '_eot_files')
      endif

      subdir_file_location = join_paths(file_location, eo_file_subdir)
      foreach mono_gen_file : mono_pub_eo_files
        if not blacklisted_files.contains(mono_gen_file)
          mono_generator_target += custom_target('eolian_mono_gen_'+mono_gen_file.underscorify()+'',
            input : join_paths(subdir_file_location, mono_gen_file),
            output : [mono_gen_file + '.cs'],
            command : [eolian_mono_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                                       '--dllimport', package_name,
                                       '-o', join_paths(meson.current_build_dir(), mono_gen_file + '.cs'),
                                       '@INPUT@'])
        endif
      endforeach
    endforeach
  endif
  if lib[1]
    subdir(package_name + '_mono')
  endif
endforeach

legacy_evas_required_by_mono = ['evas_box.eo', 'evas_image.eo', 'evas_table.eo', 'evas_text.eo']
subdir_file_location = join_paths('..', '..', 'lib', 'evas', 'canvas')
foreach mono_gen_file : legacy_evas_required_by_mono
  mono_generator_target += custom_target('eolian_mono_gen_'+mono_gen_file.underscorify()+'',
            input : join_paths(subdir_file_location, mono_gen_file),
            output : [mono_gen_file + '.cs'],
            command : [eolian_mono_gen, '-I', meson.current_source_dir(), eolian_include_directories,
                                       '--dllimport', 'evas',
                                       '-o', join_paths(meson.current_build_dir(), mono_gen_file + '.cs'),
                                       '@INPUT@'])
endforeach

efl_mono = library('efl_mono',
    mono_generator_target + mono_files + [efl_src],
    install : true,
    install_dir : join_paths(dir_lib, 'efl-mono-'+version_major)
)

efl_mono_test_suite_path=join_paths(meson.current_build_dir())

pkgconfig.generate(
  name : 'efl-mono',
  description : 'Efl C# bindings',
  version : version_major + '.' + version_minor,
  libraries : ['-r:${assemblies_dir}/efl_mono.dll'],
  variables : ['assemblies_dir='+join_paths(dir_lib, 'efl-mono-'+version_major),
               'mono_libs=-r:${assemblies_dir}/efl_mono.dll']
)
test_dirs += 'efl_mono'

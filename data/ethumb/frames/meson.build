ethumb_frame = files('default.edc')

custom_target('edje_cc_ethumb_frame',
    input : ethumb_frame,
    output : '@BASENAME@.edj',
    command : ['/usr/bin/env', 'EFL_RUN_IN_TREE=1', edje_cc.full_path(), '-beta',
              '-sd', join_paths(meson.current_source_dir()),
              '-id', join_paths(meson.current_source_dir()),
              '-fd', join_paths(meson.current_source_dir()),
              '@INPUT@', '@OUTPUT@'],
    depends : edje_cc,
    install : true,
    install_dir : join_paths(dir_data, 'ethumb', 'frames'),
)

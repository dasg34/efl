ecore_drm2_deps = [eeze, elput, dl, libdrm]
ecore_drm2_pub_deps = [ecore]

ecore_drm2_header_src = [
  'Ecore_Drm2.h'
]

ecore_drm2_src = [
  'ecore_drm2_plane.c',
  'ecore_drm2_fb.c',
  'ecore_drm2_outputs.c',
  'ecore_drm2_device.c',
  'ecore_drm2.c',
  'ecore_drm2_private.h'
]

ecore_drm2_lib = library('ecore_drm2',
    ecore_drm2_src, pub_eo_file_target,
    dependencies: ecore_drm2_pub_deps + ecore_drm2_deps,
    include_directories : config_dir,
    install: true,
    version : meson.project_version()
)

ecore_drm2 = declare_dependency(
  include_directories: [include_directories('.')],
  link_with: ecore_drm2_lib,
  sources : pub_eo_file_target + priv_eo_file_target,
  dependencies: ecore_drm2_pub_deps,
)

install_headers(ecore_drm2_header_src,
  install_dir : dir_package_include,
)

eo_cxx_suite_deps = [check]

eo_cxx_suite_src = [
  'eo_cxx_suite.cc',
  'eo_cxx_suite.h'.
]


eo_cxx_suite = executable('eo_cxx_suite',
  eo_cxx_suite_src,
  dependencies: [eo_cxx_suite_deps, check],
  c_args : [
  '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test('eo_cxx-suite', eo_cxx_suite,
  timeout : 10*60,
  env : test_env
)

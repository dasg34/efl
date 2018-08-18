eina_cxx_suite_deps = [check]

eina_cxx_suite_src = [
  'eina_cxx_suite.cc',
  'eina_cxx_test_inlist.cc',
  'eina_cxx_test_log.cc',
  'eina_cxx_test_inarray.cc',
  'eina_cxx_test_iterator.cc',
  'eina_cxx_test_ptrarray.cc',
  'eina_cxx_test_ptrlist.cc',
  'eina_cxx_test_stringshare.cc',
  'eina_cxx_test_error.cc',
  'eina_cxx_test_accessor.cc',
  'eina_cxx_test_thread.cc',
  'eina_cxx_test_optional.cc',
  'eina_cxx_test_value.cc',
  'simple.c',
  'eina_cxx_suite.h'
]


eina_cxx_suite = executable('eina_cxx_suite',
  eina_cxx_suite_src,
  dependencies: [eina_cxx_suite_deps, check],
  c_args : [
  '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test('eina_cxx-suite', eina_cxx_suite,
  timeout : 10*60,
  env : test_env
)

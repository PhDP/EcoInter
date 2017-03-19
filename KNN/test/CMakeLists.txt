set(test_cc
  run_all.cc
  set.cc
  tanimoto.cc
  symat.cc
)

source_group("Test Source" FILES ${test_cc})

add_executable(run_tests ${test_cc})

target_link_libraries(run_tests
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)

add_test(
  NAME tests
  COMMAND run_tests
)

# reference: https://google.github.io/googletest/quickstart-cmake.html

include(GoogleTest)
include_directories(../src/)

function(add_test name) 
  add_executable(
    ${name}
    ${name}.cxx
  )
  target_link_libraries(
    ${name}
    GTest::gtest_main
    Orderbook
  )
  gtest_discover_tests(${name})
endfunction()

add_subdirectory(orderbook)

add_test(TestMain)
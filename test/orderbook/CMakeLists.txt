set(LIB_NAME "OrderbookTest")

file(GLOB LIB_SOURCES *.cxx)
file(GLOB LIB_HEADERS *.hxx)

add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_HEADERS})

target_link_libraries(OrderbookTest
  PRIVATE
    GTest::gtest_main
    Orderbook
)

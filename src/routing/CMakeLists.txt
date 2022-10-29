set(LIB_NAME "Routing")

file(GLOB LIB_SOURCES *.cxx)
file(GLOB LIB_HEADERS *.hxx)

add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_HEADERS})

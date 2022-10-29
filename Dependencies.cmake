include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
 
FetchContent_MakeAvailable(googletest)

# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)

# find_package(PkgConfig REQUIRED)
# 
# FetchContent_Declare(
#   Libevent
#   GIT_REPOSITORY https://github.com/libevent/libevent.git
#   GIT_TAG release-2.1.12-stable
# )
 
# FetchContent_MakeAvailable(Libevent)

# find_package(Libevent REQUIRED COMPONENTS core pthreads)

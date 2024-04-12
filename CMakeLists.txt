cmake_minimum_required(VERSION 3.12)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cppserver C CXX ASM)

include(FetchContent)

# Fetch Boost
set(BOOST_VERSION 1.83.0) # Define the Boost version
FetchContent_Declare(
  boost
  URL https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_1_83_0.tar.bz2
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(boost)
  set(BOOST_ROOT ${boost_SOURCE_DIR})
endif()

# Fetch Google Test and Google Mock
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Specify the required Boost components
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS program_options)

# Locate MySQL include directory and library
find_path(MYSQL_INCLUDE_DIR mysql.h
          PATHS /usr/include /usr/local/include
                /usr/include/mysql /usr/local/include/mysql
                PATH_SUFFIXES mysql)
find_library(MYSQL_LIBRARY NAMES mysqlclient
             PATHS /usr/lib /usr/local/lib
                   /usr/lib/mysql /usr/local/lib/mysql)

# Includes
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/libcppserver
    ${CMAKE_SOURCE_DIR}/src/cppserverd
    ${MYSQL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

file(GLOB libcppserver_SOURCES ${CMAKE_SOURCE_DIR}/src/libcppserver/*.cpp)
file(GLOB cppserverd_SOURCES ${CMAKE_SOURCE_DIR}/src/cppserverd/*.cpp)
file(GLOB cppserver_test_SOURCES ${CMAKE_SOURCE_DIR}/src/tests/*.cpp)

# Dependencies
find_package(Threads REQUIRED)

# libcppserver library
add_library(cppserver ${libcppserver_SOURCES})
target_link_libraries(cppserver
    Threads::Threads
    Boost::program_options
    ${MYSQL_LIBRARY}
)
install(TARGETS cppserver LIBRARY DESTINATION lib)

# cppserverd executable
add_executable(cppserverd ${cppserverd_SOURCES})
target_link_libraries(cppserverd
    cppserver
)
install(TARGETS cppserverd DESTINATION bin)

# Test executable
add_executable(cppserver_tests ${cppserver_test_SOURCES})
target_link_libraries(cppserver_tests gmock_main cppserver)

include(GoogleTest)
gtest_discover_tests(cppserver_tests)

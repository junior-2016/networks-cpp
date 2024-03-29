cmake_minimum_required(VERSION 3.12)

# append other cmake module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# include GetVersion Module
include(GetVersion)

# c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(networks_cpp VERSION ${DETECTED_VERSION} LANGUAGES CXX)

# compiler warning/error flags
set(COVERAGE_COMPILE_FLAGS "-Wall -Wextra -pedantic -Werror=return-type")

# release optimization
set(CMAKE_CXX_FLAGS "-Ofast -DNDEBUG")
# debug optimization
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")

# release flags (-march=native enable all instruction subsets for local cpu. -flto open link-time-optimizaiton.)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILE_FLAGS} -fopenmp -m64 -march=native -flto")
# debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COVERAGE_COMPILE_FLAGS} -fopenmp -m64")

# use libc++ library if using clang++ compiler.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DUSE_CLANG_COMPILER) # 添加 USE_CLANG_COMPILER 宏定义.注意添加宏的语法是 -Dmacro.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # 使用 llvm 的 libc++ 标准库
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -stdlib=libc++")
endif ()

# git submodule init
find_package(Git QUIET)
message(STATUS "Submodule Init")
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)
if (NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif ()

# source file path
file(GLOB self_source_file "src/*.cpp" "src/include/*.h")
file(GLOB test_source_file "test/*.cpp" "test/*.h")
file(GLOB benchmark_source_file "benchmark/*.cpp" "benchmark/*.h")

# require libraries
set(REQUIRE_LIBS -pthread -lemon)

# find arrayfire library
find_package(ArrayFire)
OPTION(USE_CUDA_BACKEND "use cuda backend for arrayfire" OFF)
OPTION(USE_OPENCL_BACKEND "use opencl backend for arrayfire" OFF)
unset(USE_CUDA_BACKEND CACHE)
unset(USE_OPENCL_BACKEND CACHE)
set(USE_CUDA_BACKEND ON)
if (USE_CUDA_BACKEND)
    set(REQUIRE_LIBS ${REQUIRE_LIBS} ArrayFire::afcuda)
elseif (USE_OPENCL_BACKEND)
    set(REQUIRE_LIBS ${REQUIRE_LIBS} ArrayFire::afopencl)
else ()
    set(REQUIRE_LIBS ${REQUIRE_LIBS} ArrayFire::afcpu)
endif ()

# benchmark/gtest setting
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
add_subdirectory(third-party/google/benchmark)
add_subdirectory(third-party/google/googletest)

# option for test and benchmark
OPTION(TEST "build test" ON)
OPTION(BENCHMARK "build benchmark" ON)
unset(TEST CACHE)
unset(BENCHMARK CACHE)
set(TEST ON)
set(BENCHMARK ON)

# create executable
if (TEST)
    add_executable("${PROJECT_NAME}_test" ${self_source_file} ${test_source_file})
    target_link_libraries("${PROJECT_NAME}_test" gtest gtest_main ${REQUIRE_LIBS})
ENDIF ()
if (BENCHMARK)
    add_executable("${PROJECT_NAME}_benchmark" ${self_source_file} ${benchmark_source_file})
    target_link_libraries("${PROJECT_NAME}_benchmark" benchmark ${REQUIRE_LIBS})
ENDIF ()

cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------------------------------------------
# Disable gcov/coverage flags BEFORE FetchContent
# Must be set before any subprojects are added
# -------------------------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

# Explicitly disable coverage for all targets
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -fno-profile-arcs -fno-test-coverage")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -fno-profile-arcs -fno-test-coverage")

# Force disable coverage options that csv-parser might set
set(CSV_BUILD_COVERAGE OFF CACHE BOOL "Disable coverage for csv-parser" FORCE)
set(ENABLE_COVERAGE OFF CACHE BOOL "Disable coverage" FORCE)

# MSVC exception flags (if using Windows)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

# -------------------------------------------------------------------
# Fetch csv-parser
# -------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  csv-parser
  GIT_REPOSITORY https://github.com/vincentlaucsb/csv-parser.git
  GIT_TAG master
)

FetchContent_MakeAvailable(csv-parser)

# -------------------------------------------------------------------
# Add your subprojects
# -------------------------------------------------------------------
add_subdirectory(core_library)

add_executable(JobResumeSearchSort src/main.cpp)

target_link_libraries(JobResumeSearchSort PRIVATE core_library csv)
add_library(core_library STATIC
src/queue.cpp
)

# Public headers: everything in include/ is visible
target_include_directories(core_library PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: internal headers in src/
target_include_directories(core_library PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(core_library PRIVATE)

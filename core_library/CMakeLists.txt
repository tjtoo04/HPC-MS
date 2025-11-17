add_library(core_library STATIC
src/queue.cpp
src/ambulance_dispatcher/ambulance_dispatcher.cpp
src/ambulance_dispatcher/ambulance.cpp
src/ambulance_dispatcher/circular_queue.cpp
src/emergency_department/emergency_case.cpp
src/emergency_department/priority_queue.cpp
src/emergency_department/emergency_officer.cpp
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

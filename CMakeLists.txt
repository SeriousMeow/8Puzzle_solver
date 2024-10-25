cmake_minimum_required(VERSION 3.10)

project(Solver)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(solver_files
    src/Solve.cpp
    src/utils.cpp
    src/State.cpp
)

set(interactive_source
    src/interactive.cpp
    ${solver_files}
)

set(test_source 
   src/test.cpp
   ${solver_files}
)

add_executable(interactive_solver ${interactive_source})
add_executable(test_solver ${test_source})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(interactive_solver PRIVATE
    -g
    -Wall
    -fsanitize=address
    )
    target_link_options(interactive_solver PRIVATE
    -fsanitize=address
    )


    target_compile_options(test_solver PRIVATE
    -g
    -Wall
    -fsanitize=address
    )
    target_link_options(test_solver PRIVATE
    -fsanitize=address
    )
endif()

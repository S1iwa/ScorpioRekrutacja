cmake_minimum_required(VERSION 3.10)

project(scorpio_recruitment_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Exercise library
add_library(backend SHARED
  src/backend/motor.cpp
)
target_include_directories(backend PUBLIC include/backend_interface/)
target_include_directories(backend PRIVATE include/backend/)


# Solution library
file(GLOB solution_SRC "src/solution/*.cpp")
add_library(solution SHARED ${solution_SRC})
target_include_directories(solution PRIVATE
  include/solution/
  include/backend_interface/
)
target_include_directories(solution PUBLIC include/solution_interface/)


# Executable for the exercise
add_executable(scorpio_recruitment_task src/main.cpp)
target_link_libraries(scorpio_recruitment_task PRIVATE backend solution)

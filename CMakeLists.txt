cmake_minimum_required(VERSION 3.15)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SDL3 path
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/libs/SDL3_install")

# Find SDL3
find_package(SDL3 3.2.1 EXACT REQUIRED)


# Add executable
add_executable(${PROJECT_NAME}
   test/sdlinit.c
#    src/main.cpp
#   src/window.cpp
    #src/test.cpp

)



# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${SDL3_INCLUDE_DIRS}
)

# Link SDL3
target_link_libraries(${PROJECT_NAME} PRIVATE
SDL3::SDL3

)
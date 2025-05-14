cmake_minimum_required(VERSION 3.10)
project(redis_app)

set(CMAKE_CXX_STANDARD 17)

find_package(redis++ REQUIRED)
find_package(hiredis REQUIRED)

add_executable(redis_app main.cpp)

target_link_libraries(redis_app PRIVATE redis++::redis++)

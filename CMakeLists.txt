cmake_minimum_required(VERSION 3.7)
project(daily_pottery_project)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DRELEASE ")

file(GLOB daily_pottery_SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

link_directories("libraries/lib")
include_directories("libraries/include")

add_executable(daily_pottery ${daily_pottery_SRC} icon.o)
target_link_options(daily_pottery PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})

target_link_libraries(daily_pottery SDL2main)
target_link_libraries(daily_pottery SDL2)
target_link_libraries(daily_pottery SDL2_image)
target_link_libraries(daily_pottery SDL2_mixer)

target_compile_features(daily_pottery PRIVATE cxx_std_20)
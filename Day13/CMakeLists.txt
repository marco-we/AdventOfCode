get_filename_component(dirname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${dirname})

file(GLOB files *.txt)
add_executable(${dirname} ${files} main.cpp)

set_target_properties(${dirname} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
cmake_minimum_required(VERSION 3.21)
project(siggraph_opengl_tutorial)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# every subfolder with a main.cpp becomes it's own executable
file(GLOB children LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/*")
foreach(dir ${children})
   # check
   if(NOT EXISTS "${dir}/main.cpp")
      continue()
   endif()
   get_filename_component(name "${dir}" NAME)
   file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${dir}/*.cpp")

   add_executable(${name} ${sources})
   target_link_libraries(${name} glad::glad glfw glm::glm)

   # full path to this folder,
   # so shaders and other files can be found no matter where the exe runs from
   target_compile_definitions(${name} PRIVATE SHADER_DIR="${dir}/")

   # bake the c++ standard library into the exe,
   # otherwise it might load a different libstdc++-6.dll from PATH and crash
   # please dont ask marcel about this he wont know :) target_link_options(${name} PRIVATE -static-libstdc++ -static-libgcc)
endforeach()

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(nlive)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_compile_definitions(__STDC_CONSTANT_MACROS)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_options(-bigobj)
if (MSVC)
  add_compile_options(/std:c++17)
  # https://studiofreya.com/2018/01/06/visual-studio-2017-with-cpp17-and-boost/
  add_compile_definitions(_HAS_STD_BYTE=0)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(Boost 1.45.0 REQUIRED
  COMPONENTS
  filesystem
  serialization
)

find_package(Qt5 REQUIRED
  COMPONENTS
  Core
  Gui
  Widgets
  Concurrent
  Multimedia
  OpenGL
  Svg
  LinguistTools
)
find_package(FFMPEG 3.4 REQUIRED
  COMPONENTS
  avutil
  avcodec
  avformat
  avfilter
  swscale
  swresample
)
find_package(spdlog REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(app)
add_subdirectory(test)
add_subdirectory(googletest)
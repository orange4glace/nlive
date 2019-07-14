cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(nlive)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
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
message(STATUS ${spdlog_INCLUDE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(app)
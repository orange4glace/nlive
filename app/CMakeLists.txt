set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  main.cpp
)

add_subdirectory(base)
add_subdirectory(model)
add_subdirectory(platform)
add_subdirectory(browser)
add_subdirectory(renderer)

add_executable(nlive
  ${NLIVE_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} BEFORE
  PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${spdlog_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
  OpenGL::GL
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Concurrent
  Qt5::Multimedia
  Qt5::OpenGL
  Qt5::Svg
  FFMPEG::avutil
  FFMPEG::avcodec
  FFMPEG::avformat
  FFMPEG::avfilter
  FFMPEG::swscale
  FFMPEG::swresample
  ${Boost_LIBRARIES}
)
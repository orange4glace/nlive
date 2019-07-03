set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  main.cpp
)

add_subdirectory(base)
add_subdirectory(model)
add_subdirectory(platform)
add_subdirectory(browser)

add_executable(nlive
  ${NLIVE_SOURCES}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE
  OpenGL::GL
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Multimedia
  Qt5::OpenGL
  Qt5::Svg
  # FFMPEG::avutil
  # FFMPEG::avcodec
  # FFMPEG::avformat
  # FFMPEG::avfilter
  # FFMPEG::swscale
  # FFMPEG::swresample
)
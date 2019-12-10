set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(NLIVE_TEST_SOURCES
  ${NLIVE_TEST_SOURCES}
  main.cpp
  storage_item.cpp
  serialize/sequence.cpp
  serialize/resource.cpp
  serialize/effect.cpp
  serialize/storage_item.cpp
  platform/context_key.cpp
  encoder/encoder_test.cpp
)

add_executable(${BINARY}
  ${NLIVE_TEST_SOURCES}
)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_include_directories(${BINARY} BEFORE
  PRIVATE
  ../app
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Concurrent_INCLUDE_DIRS}
  ${Qt5Multimedia_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
  ${Qt5Svg_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${FFMPEG_INCLUDE_DIRS}
  ${spdlog_INCLUDE_DIR}
)

target_link_libraries(${BINARY}
  PUBLIC
  nlive_test_lib
  gtest
)
add_subdirectory(drag)
add_subdirectory(widgets)
add_subdirectory(services)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/main_window.h
  browser/main_window.cpp
  PARENT_SCOPE
)
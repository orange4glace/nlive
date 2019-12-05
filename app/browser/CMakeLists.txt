add_subdirectory(drag)
add_subdirectory(widgets)
add_subdirectory(menu_bar)
add_subdirectory(services)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets_service.h
  browser/widgets_service.cpp
  browser/main_window.h
  browser/main_window.cpp
  PARENT_SCOPE
)
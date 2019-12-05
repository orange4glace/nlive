add_subdirectory(action)
add_subdirectory(logger)
add_subdirectory(service)
add_subdirectory(theme)
add_subdirectory(task)
add_subdirectory(resource)
add_subdirectory(undo)
add_subdirectory(context_key)
add_subdirectory(commands)
# add_subdirectory(menu_bar)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  platform/include.h
  PARENT_SCOPE
)
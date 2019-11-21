add_subdirectory(logger)
add_subdirectory(service)
add_subdirectory(theme)
add_subdirectory(task)
add_subdirectory(resource)
add_subdirectory(undo)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  PARENT_SCOPE
)
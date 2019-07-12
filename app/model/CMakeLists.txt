add_subdirectory(common)
add_subdirectory(sequence)
add_subdirectory(storage)
add_subdirectory(resource)
add_subdirectory(project)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  PARENT_SCOPE
)
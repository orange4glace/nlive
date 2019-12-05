add_subdirectory(common)
add_subdirectory(sequence)
add_subdirectory(storage)
add_subdirectory(resource)
add_subdirectory(project)
add_subdirectory(effect)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  model/include.h
  model/serialization.h
  PARENT_SCOPE
)
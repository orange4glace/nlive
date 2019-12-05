add_subdirectory(widgets)
add_subdirectory(projects)
add_subdirectory(menu_bar)
add_subdirectory(import)
add_subdirectory(play)
add_subdirectory(memento)
add_subdirectory(audio_flaty)
add_subdirectory(commands)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/services/include.h
  PARENT_SCOPE
)
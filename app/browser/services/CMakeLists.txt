add_subdirectory(widgets)
add_subdirectory(sequences)
add_subdirectory(projects)
add_subdirectory(menu_bar)
add_subdirectory(import)
add_subdirectory(play)
add_subdirectory(memento)
add_subdirectory(audio_flaty)
add_subdirectory(commands)
add_subdirectory(context_key)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/services/include.h
  PARENT_SCOPE
)
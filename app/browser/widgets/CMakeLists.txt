add_subdirectory(timeline)
add_subdirectory(project)
add_subdirectory(monitor)
add_subdirectory(effect_control)
add_subdirectory(task_bar)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets/widget.h
  PARENT_SCOPE
)
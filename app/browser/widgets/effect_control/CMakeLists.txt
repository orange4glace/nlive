add_subdirectory(property)
add_subdirectory(transform)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets/effect_control/effect_control_widget.h
  browser/widgets/effect_control/effect_control_widget.cpp
  browser/widgets/effect_control/effect_control_layout.h
  browser/widgets/effect_control/sequence_view.h
  browser/widgets/effect_control/sequence_view.cpp
  browser/widgets/effect_control/clip_view.h
  browser/widgets/effect_control/clip_view.cpp
  browser/widgets/effect_control/effect_view.h
  browser/widgets/effect_control/effect_view.cpp
  browser/widgets/effect_control/effect_view_factory.h
  browser/widgets/effect_control/effect_view_factory.cpp
  PARENT_SCOPE
)
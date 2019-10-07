add_subdirectory(value)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  model/effect/keyframe.h
  model/effect/property.h
  model/effect/effect.h
  model/effect/effect.cpp
  model/effect/transform_effect.h
  model/effect/transform_effect.cpp
  PARENT_SCOPE
)
add_subdirectory(scrollview)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets/timeline/timelinewidget.h
  browser/widgets/timeline/timelinewidget.cpp

  browser/widgets/timeline/sequenceview.h
  browser/widgets/timeline/sequenceview.cpp

  browser/widgets/timeline/sequencesideview.h
  browser/widgets/timeline/sequencesideview.cpp

  browser/widgets/timeline/sequencetimelineview.h
  browser/widgets/timeline/sequencetimelineview.cpp

  browser/widgets/timeline/trackview.h
  browser/widgets/timeline/trackview.cpp

  browser/widgets/timeline/tracksideview.h
  browser/widgets/timeline/tracksideview.cpp

  browser/widgets/timeline/tracktimelineview.h
  browser/widgets/timeline/tracktimelineview.cpp

  browser/widgets/timeline/clipview.h
  browser/widgets/timeline/clipview.cpp
  PARENT_SCOPE
)
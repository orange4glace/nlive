add_subdirectory(scrollview)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets/timeline/sequence_playable.h
  browser/widgets/timeline/sequence_playable.cpp

  browser/widgets/timeline/timeline_widget.h
  browser/widgets/timeline/timeline_widget.cpp

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

  browser/widgets/timeline/ghost_sequence_view.h
  browser/widgets/timeline/ghost_sequence_view.cpp

  browser/widgets/timeline/ghost_track_view.h
  browser/widgets/timeline/ghost_track_view.cpp

  browser/widgets/timeline/ghost_clip_view.h
  browser/widgets/timeline/ghost_clip_view.cpp

  browser/widgets/timeline/timeline_widget_service.h
  browser/widgets/timeline/timeline_widget_service_impl.h
  browser/widgets/timeline/timeline_widget_service_impl.cpp

  PARENT_SCOPE
)
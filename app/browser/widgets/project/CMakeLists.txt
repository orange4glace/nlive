add_subdirectory(storage_item_view)

set(NLIVE_SOURCES
  ${NLIVE_SOURCES}
  browser/widgets/project/project_widget.h
  browser/widgets/project/project_widget.cpp
  browser/widgets/project/directory_view.h
  browser/widgets/project/directory_view.cpp
  browser/widgets/project/storage_item_view.h
  browser/widgets/project/storage_item_view.cpp
  browser/widgets/project/storage_item_view_factory.h
  browser/widgets/project/storage_item_view_factory.cpp
  browser/widgets/project/null_storage_item_view.h
  browser/widgets/project/null_storage_item_view.cpp
  browser/widgets/project/video_storage_item_view.h
  browser/widgets/project/video_storage_item_view.cpp
  PARENT_SCOPE
)
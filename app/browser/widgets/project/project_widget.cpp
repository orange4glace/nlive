#include "browser/widgets/project/project_widget.h"

#include "base/layout/flex_layout.h"
#include "base/layout/fillparentbox.h"
#include "base/ui/solid_box.h"
#include "platform/theme/themeservice.h"
#include "model/storage/storage_directory.h"
#include "browser/widgets/project/directory_view.h"

#include "browser/widgets/project/storage_item_view_factory.h"
#include "browser/widgets/project/null_storage_item_view.h"
#include "browser/widgets/project/video_resource_storage_item_view.h"

#include <QDebug>

namespace nlive {

namespace project_widget {

void ProjectWidget::Initialize() {
  // Register pre-defined views
  StorageItemViewFactoryRegistry::registerDefaultFactory(new NullStorageItemViewFactory());
  StorageItemViewFactoryRegistry::registerFactory(VideoResourceStorageItem::TYPE, new VideoResourceStorageItemViewFactory());

}

ProjectWidget::ProjectWidget(QWidget* parent,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IImportService> import_service) :
  QDockWidget(parent),
  theme_service_(theme_service),
  import_service_(import_service),
  directory_view_(nullptr) {
  setTitleBarWidget(new QWidget());

  FlexLayout* header = new FlexLayout(this);
  SolidBox* btn1 = new SolidBox(header);
  SolidBox* btn2 = new SolidBox(header, Qt::darkBlue);
  btn1->setFlex(2);
  btn2->setFlex(1);
  header->addChild(btn1);
  header->addChild(btn2);
  header->resize(width(), 30);
  header->raise();
}

void ProjectWidget::setDirectory(QSharedPointer<StorageDirectory> directory) {
  qDebug() << "Set Directory " << directory.get() << "\n";
  if (directory_ == directory) return;
  if (directory_ != nullptr) {
    // Clean up
    delete directory_view_;
  }
  auto directory_view = new DirectoryView(this, directory, theme_service_, import_service_);
  directory_view_ = directory_view;
}

void ProjectWidget::resizeEvent(QResizeEvent* event) {
  if (directory_view_ != nullptr)
    directory_view_->setGeometry(rect());
}

}

}
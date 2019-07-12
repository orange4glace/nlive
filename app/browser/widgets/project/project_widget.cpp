#include "browser/widgets/project/project_widget.h"

#include "platform/theme/themeservice.h"
#include "model/storage/storage_directory.h"
#include "browser/widgets/project/directory_view.h"

#include "browser/widgets/project/storage_item_view_factory.h"
#include "browser/widgets/project/null_storage_item_view.h"

#include <QDebug>

namespace nlive {

namespace project_widget {

void ProjectWidget::Initialize() {
  // Register pre-defined views
  StorageItemViewFactoryRegistry::registerDefaultFactory(new NullStorageItemViewFactory());

}

ProjectWidget::ProjectWidget(QWidget* parent,
    IThemeService* theme_service,
    IImportService* import_service) :
  theme_service_(theme_service),
  import_service_(import_service),
  QDockWidget(parent),
  directory_view_(nullptr) {

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
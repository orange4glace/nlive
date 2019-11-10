#include "browser/widgets/project/project_widget.h"

#include "base/layout/flex_layout.h"
#include "base/layout/fillparentbox.h"
#include "base/ui/solid_box.h"
#include "base/ui/svg_button.h"
#include "platform/theme/themeservice.h"
#include "model/storage/storage_directory.h"
#include "browser/widgets/project/directory_view.h"

#include "browser/widgets/project/storage_item_view_factory.h"
#include "browser/widgets/project/null_storage_item_view.h"
#include "browser/widgets/project/video_resource_storage_item_view.h"
#include "browser/widgets/project/storage_item_view/audio_storage_item_view.h"

#include <QDebug>

namespace nlive {

namespace project_widget {

void ProjectWidget::Initialize() {
  // Register pre-defined views
  StorageItemViewFactoryRegistry::registerDefaultFactory(new NullStorageItemViewFactory());
  StorageItemViewFactoryRegistry::registerFactory(VideoResourceStorageItem::TYPE, new VideoResourceStorageItemViewFactory());
  StorageItemViewFactoryRegistry::registerFactory(AudioStorageItem::TYPE, new AudioStorageItemViewFactory());

}

ProjectWidget::ProjectWidget(QWidget* parent,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IImportService> import_service) :
  QDockWidget(parent),
  theme_service_(theme_service),
  import_service_(import_service),
  directory_view_(nullptr) {
  setTitleBarWidget(new QWidget());

  outer_view_ = new FlexLayout(nullptr, FlexLayout::Direction::Row);
  container_ = new FillParentBox(this, outer_view_);
  container_->setPadding(Div::ALL, 15);

  FlexLayout* header = new FlexLayout(this);
  header->setPadding(Div::TOP, 5)->setPadding(Div::BOTTOM, 5);
  SvgButton* btn1 = new SvgButton(header, ":/widget/project/folder.svg");
  btn1->setFlexBasis(20)->setFlexGrow(0)->setFlexShrink(0);
  directory_path_view_ = new TextBox(header, "");
  directory_path_view_->setFlag(directory_path_view_->flag() | Qt::AlignVCenter)->setPadding(Div::LEFT, 10);
  directory_path_view_->setColor(theme_service_->getTheme().surfaceTextColor());
  header->addChild(btn1);
  header->addChild(directory_path_view_);

  header->setFlexBasis(30)->setFlexGrow(0)->setFlexShrink(0);
  outer_view_->addChild(header);

}

void ProjectWidget::setDirectory(QSharedPointer<StorageDirectory> directory) {
  if (directory_ == directory) return;
  if (directory_ != nullptr) {
    // Clean up
    outer_view_->removeChild(directory_view_);
    delete directory_view_;
  }
  if (directory == nullptr) return;
  directory_ = directory;
  auto directory_view = new DirectoryView(outer_view_, directory, theme_service_, import_service_);
  outer_view_->addChild(directory_view);
  directory_view_ = directory_view;
  directory_path_view_->setText(directory_->getAbsoluteNamePath());
}

void ProjectWidget::resizeEvent(QResizeEvent* event) {
  container_->setGeometry(rect());
  // if (directory_view_ != nullptr)
  //   directory_view_->setGeometry(rect());
}

}

}
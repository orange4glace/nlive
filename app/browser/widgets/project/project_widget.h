#ifndef NLIVE_PROJECT_WIDGET_H_
#define NLIVE_PROJECT_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include "browser/widgets/widget.h"
#include "base/layout/flex_layout.h"
#include "base/layout/fillparentbox.h"
#include "base/ui/text_box.h"
#include "platform/theme/themeservice.h"
#include "platform/service/service_locator.h"

namespace nlive {

class IImportService;
class StorageDirectory;

namespace project_widget {

class DirectoryView;

class ProjectWidget : public QDockWidget, public Widget {
  
public:
  static const std::string TYPE;

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<IImportService> import_service_;
  QSharedPointer<ServiceLocator> service_locator_;

  QSharedPointer<StorageDirectory> directory_;

  FillParentBox* container_;
  FlexLayout* outer_view_;
  TextBox* directory_path_view_;
  DirectoryView* directory_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  static void Initialize(QSharedPointer<ServiceLocator> service_locator);

  ProjectWidget(QWidget* parent,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IImportService> import_service,
    QSharedPointer<ServiceLocator> service_locator);

  void setDirectory(QSharedPointer<StorageDirectory> directory);

  inline std::string name() const override { return TYPE; }

};

}

}

#endif
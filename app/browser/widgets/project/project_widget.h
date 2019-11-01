#ifndef _NLIVE_PROJECT_WIDGET_H_
#define _NLIVE_PROJECT_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>
#include "base/layout/flex_layout.h"
#include "base/ui/text_box.h"
#include "platform/theme/themeservice.h"

namespace nlive {

class IImportService;
class StorageDirectory;

namespace project_widget {

class DirectoryView;

class ProjectWidget : public QDockWidget {

private:
  QSharedPointer<IThemeService> theme_service_;
  QSharedPointer<IImportService> import_service_;

  QSharedPointer<StorageDirectory> directory_;

  FlexLayout* outer_view_;
  TextBox* directory_path_view_;
  DirectoryView* directory_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  static void Initialize();

  ProjectWidget(QWidget* parent,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IImportService> import_service);

  void setDirectory(QSharedPointer<StorageDirectory> directory);

};

}

}

#endif
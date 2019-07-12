#ifndef _NLIVE_PROJECT_WIDGET_H_
#define _NLIVE_PROJECT_WIDGET_H_

#include <QDockWidget>
#include <QSharedPointer>

namespace nlive {

class IThemeService;
class IImportService;
class StorageDirectory;

namespace project_widget {

class DirectoryView;

class ProjectWidget : public QDockWidget {

private:
  IThemeService* theme_service_;
  IImportService* import_service_;

  QSharedPointer<StorageDirectory> directory_;
  DirectoryView* directory_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  static void Initialize();

  ProjectWidget(QWidget* parent,
    IThemeService* theme_service,
    IImportService* import_service);

  void setDirectory(QSharedPointer<StorageDirectory> directory);

};

}

}

#endif
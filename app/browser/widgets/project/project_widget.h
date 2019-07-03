#ifndef _NLIVE_PROJECT_WIDGET_H_
#define _NLIVE_PROJECT_WIDGET_H_

#include <QDockWidget>

namespace nlive {

class IThemeService;
class StorageDirectory;

namespace project_widget {

class DirectoryView;

class ProjectWidget : public QDockWidget {

private:
  IThemeService* theme_service_;

  StorageDirectory* directory_;
  DirectoryView* directory_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  ProjectWidget(QWidget* parent, IThemeService* theme_service);

  void setDirectory(StorageDirectory* directory);

};

}

}

#endif
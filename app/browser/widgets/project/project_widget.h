#ifndef NLIVE_PROJECT_WIDGET_H_
#define NLIVE_PROJECT_WIDGET_H_

#include <QDockWidget>
#include "base/common/memory.h"
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
  sptr<IThemeService> theme_service_;
  sptr<IImportService> import_service_;
  sptr<ServiceLocator> service_locator_;

  sptr<StorageDirectory> directory_;

  FillParentBox* container_;
  FlexLayout* outer_view_;
  TextBox* directory_path_view_;
  DirectoryView* directory_view_;

protected:
  void resizeEvent(QResizeEvent* event) override;

public:
  static void Initialize(sptr<ServiceLocator> service_locator);

  ProjectWidget(QWidget* parent,
    sptr<IThemeService> theme_service,
    sptr<IImportService> import_service,
    sptr<ServiceLocator> service_locator);

  void setDirectory(sptr<StorageDirectory> directory);

  inline std::string name() const override { return TYPE; }

};

}

}

#endif
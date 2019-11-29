#ifndef NLIVE_WIDGET_H_
#define NLIVE_WIDGET_H_

#include <string>
#include <QDockWidget>
#include "base/common/memory.h"
#include "platform/theme/themeservice.h"

namespace nlive {

class Widget : public QDockWidget {

protected:
  sptr<IThemeService> theme_service_;
  
  void paintEvent(QPaintEvent* e) override;

public:
  Widget(QWidget* parent, sptr<IThemeService> theme_service);

  virtual std::string name() const = 0;

};

}

#endif 
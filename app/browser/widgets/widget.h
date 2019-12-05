#ifndef NLIVE_WIDGET_H_
#define NLIVE_WIDGET_H_

#include <string>
#include <QDockWidget>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "platform/theme/themeservice.h"

namespace nlive {

class Widget : public QDockWidget, public Sig {

private:
  bool focused_;

protected:
  sptr<IThemeService> theme_service_;
  
  void paintEvent(QPaintEvent* e) override;
  void focusInEvent(QFocusEvent* e) override;
  void focusOutEvent(QFocusEvent* e) override;
  virtual void onFocused();
  virtual void onBlured();

public:
  Widget(QWidget* parent, sptr<IThemeService> theme_service);

  bool focused() const;

  virtual std::string name() const = 0;

  sig2_t<void ()> onDidFocus;
  sig2_t<void ()> onDidBlur;

};

}

#endif 
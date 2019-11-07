#ifndef NLIVE_ITHEME_SERVICE_IMPL_H_
#define NLIVE_ITHEME_SERVICE_IMPL_H_

#include <QSharedPointer>
#include "platform/theme/themeservice.h"

namespace nlive {

class ThemeService : public IThemeService {

private:
  static QSharedPointer<ThemeService> instance_;

  Theme theme_;

protected:
  ThemeService();

public:
  static void Initialize();
  inline static QSharedPointer<ThemeService> const instance() {
    return ThemeService::instance_;
  }

  void setTheme(Theme const& theme) override;
  const Theme& getTheme() const override;

};

}

#endif
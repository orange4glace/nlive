#ifndef _NLIVE_ITHEME_SERVICE_H_
#define _NLIVE_ITHEME_SERVICE_H_

#include <QObject>
#include "platform/theme/theme.h"

namespace nlive {

class IThemeService : public QObject {
  Q_OBJECT

protected:
  inline IThemeService() {}

public:
  virtual void setTheme(Theme const& theme) = 0;
  virtual const Theme& getTheme() const = 0;

signals:
  void onDidChangeTheme(Theme const& theme) const;

};

}

#endif
#ifndef _NLIVE_ITHEME_SERVICE_H_
#define _NLIVE_ITHEME_SERVICE_H_

#include <QObject>
#include "base/common/sig.h"
#include "platform/theme/theme.h"

namespace nlive {

class IThemeService : public Sig {

protected:
  inline IThemeService() {}

public:
  virtual void setTheme(Theme const& theme) = 0;
  virtual const Theme& getTheme() const = 0;

  sig2_t<void ()> onDidUpdate;

};

}

#endif
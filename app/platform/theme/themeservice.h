#ifndef NLIVE_ITHEME_SERVICE_H_
#define NLIVE_ITHEME_SERVICE_H_

#include <QObject>
#include "base/common/sig.h"
#include "platform/service/service.h"
#include "platform/theme/theme.h"

namespace nlive {

class IThemeService : public IService, public Sig {

public:
  static const std::string ID;

protected:
  inline IThemeService() {}

public:
  virtual void setTheme(Theme const& theme) = 0;
  virtual const Theme& getTheme() const = 0;

  sig2_t<void ()> onDidUpdate;

  inline std::string service_id() const override { return ID; }

};

}

#endif
#ifndef _NLIVE_EFFECT_H_
#define _NLIVE_EFFECT_H_

#include <QSharedPointer>
#include <string>
#include "base/common/sig.h"

namespace nlive {

namespace effect {

class Effect : public Sig {

private:
  std::string type_;

public:
  Effect(std::string type) : type_(type) {

  }

  const std::string& type() const {
    return type_;
  }

  template <class T>
  void connectProperty(QSharedPointer<T> property) {
    property->onDidUpdate.connect(
      sig2_t<void (void)>::slot_type([this] {
      onDidUpdate();
    }));
  }

  sig2_t<void (void)> onDidUpdate;

};

struct EffectCompare {
  using is_transparent = void;
  inline bool operator() (const QSharedPointer<Effect>& a, const QSharedPointer<Effect>& b) const
  { return a.get() < b.get(); }
  inline bool operator() (const QSharedPointer<Effect>& a, const Effect* b) const
  { return a.get() < b; }
  inline bool operator() (const Effect* a, const QSharedPointer<Effect>& b) const
  { return a < b.get(); }
};

}

}

#endif
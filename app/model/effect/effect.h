#ifndef _NLIVE_EFFECT_H_
#define _NLIVE_EFFECT_H_

#include <QSharedPointer>
#include <string>

namespace nlive {

namespace effect {

class Effect {

private:
  std::string type_;

public:
  Effect(std::string type);
  const std::string& type() const;

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
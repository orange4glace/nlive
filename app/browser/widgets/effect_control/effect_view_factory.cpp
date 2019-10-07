#include "browser/widgets/effect_control/effect_view_factory.h"

namespace nlive {

namespace effect_control {

EffectViewFactory* EffectViewFactoryRegistry::default_factory_ = nullptr;
std::map<std::string, EffectViewFactory*> EffectViewFactoryRegistry::factories_;

EffectViewFactory::EffectViewFactory() {}

void EffectViewFactoryRegistry::registerDefaultFactory(EffectViewFactory* factory) {
  default_factory_ = factory;
}

void EffectViewFactoryRegistry::registerFactory(std::string type, EffectViewFactory* factory) {
  factories_.emplace(type, factory);
}

EffectViewFactory* EffectViewFactoryRegistry::getFactory(std::string type) {
  if (factories_.count(type) == 0) return default_factory_;
  return factories_[type];
}

}

}
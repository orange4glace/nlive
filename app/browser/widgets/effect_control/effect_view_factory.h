#ifndef NLIVE_EFFECT_CONTROL_EFFECT_VIEW_FACTORY_H_
#define NLIVE_EFFECT_CONTROL_EFFECT_VIEW_FACTORY_H_

#include <QSharedPointer>
#include <string>
#include <map>

#include "platform/theme/themeservice.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/timeline/sequenceview.h"

class QWidget;

namespace nlive {

class Sequence;
class Clip;

namespace effect {

class Effect;

}

namespace effect_control {

class EffectControlLayout;
class EffectView;

class EffectViewFactory {

public:
  EffectViewFactory();
  virtual EffectView* create(
    QWidget* parent,
    QSharedPointer<EffectControlLayout> layout_params,
    QSharedPointer<Sequence> sequence,
    QSharedPointer<Clip> clip,
    QSharedPointer<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    QSharedPointer<IThemeService> theme_service,
    QSharedPointer<IMementoService> memento_service) = 0;

};

class EffectViewFactoryRegistry {

private:
  static EffectViewFactory* default_factory_;
  static std::map<std::string, EffectViewFactory*> factories_;

public:
  static void registerDefaultFactory(EffectViewFactory* factory);
  static void registerFactory(std::string type, EffectViewFactory* factory);
  static EffectViewFactory* getFactory(std::string type);

};

}

}

#endif
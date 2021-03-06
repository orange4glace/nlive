#ifndef NLIVE_EFFECT_CONTROL_EFFECT_VIEW_FACTORY_H_
#define NLIVE_EFFECT_CONTROL_EFFECT_VIEW_FACTORY_H_

#include "base/common/memory.h"
#include <string>
#include <map>

#include "platform/theme/themeservice.h"
#include "browser/services/memento/memento_service.h"
#include "browser/widgets/timeline/sequenceview.h"
#include "browser/widgets/effect_control/keyframes_controller.h"

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
    sptr<IKeyframesController> keyframes_controller,
    sptr<EffectControlLayout> layout_params,
    sptr<Sequence> sequence,
    sptr<Clip> clip,
    sptr<effect::Effect> effect,
    SequenceScrollView* sequence_scroll_view,
    sptr<IThemeService> theme_service,
    sptr<IMementoService> memento_service) = 0;

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
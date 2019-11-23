#ifndef NLIVE_EFFECT_H_
#define NLIVE_EFFECT_H_

#include <string>
#include "base/common/memory.h"
#include <QDebug>
#include "base/common/sig.h"
#include "base/common/uuid.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace effect {

class Effect : public Sig {

private:
  std::string id_;
  std::string type_;

public:
  inline Effect(std::string type) : type_(type) {
    id_ = UUID::instance()->generateUUID();
    qDebug() << QString::fromStdString(id_);
  }

  inline virtual void render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset) {}

  const std::string& id() const { return id_; }
  const std::string& type() const { return type_; }

  template <class T>
  void connectProperty(sptr<T> property) {
    property->onDidUpdate.connect(
      sig2_t<void (void)>::slot_type([this] {
      onDidUpdate();
    }));
  }

  sig2_t<void (void)> onDidUpdate;

};

struct EffectCompare {
  using is_transparent = void;
  inline bool operator() (const sptr<Effect>& a, const sptr<Effect>& b) const
  { return a.get() < b.get(); }
  inline bool operator() (const sptr<Effect>& a, const Effect* b) const
  { return a.get() < b; }
  inline bool operator() (const Effect* a, const sptr<Effect>& b) const
  { return a < b.get(); }
};

}

}

#endif
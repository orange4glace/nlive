#ifndef NLIVE_EFFECT_H_
#define NLIVE_EFFECT_H_

#include <string>
#include "base/common/memory.h"
#include <QDebug>
#include "base/common/sig.h"
#include "base/common/uuid.h"
#include "base/common/serialize.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace effect {

class Effect : public Sig {

private:
  Effect() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & id_ & type_;
  }

  std::string id_;
  std::string type_;

public:
  Effect(std::string type);

  virtual void render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset);

  const std::string& id() const;
  const std::string& type() const;

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
  bool operator() (const sptr<Effect>& a, const sptr<Effect>& b) const;
  bool operator() (const sptr<Effect>& a, const Effect* b) const;
  bool operator() (const Effect* a, const sptr<Effect>& b) const;
};

}

}

#endif
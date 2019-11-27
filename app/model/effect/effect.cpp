#include "model/effect/effect.h"

namespace nlive {

namespace effect {

Effect::Effect(std::string type) : type_(type) {
  id_ = UUID::instance()->generateUUID();
}

void Effect::render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset) {}

const std::string& Effect::id() const { return id_; }
const std::string& Effect::type() const { return type_; }

bool EffectCompare::operator() (const sptr<Effect>& a, const sptr<Effect>& b) const
{ return a.get() < b.get(); }
bool EffectCompare::operator() (const sptr<Effect>& a, const Effect* b) const
{ return a.get() < b; }
bool EffectCompare::operator() (const Effect* a, const sptr<Effect>& b) const
{ return a < b.get(); }

}

}
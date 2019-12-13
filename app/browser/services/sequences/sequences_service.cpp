#include "browser/services/sequences/sequences_service.h"

#include "platform/context_key/context_key.h"

namespace nlive {

const sptr<RawContextKey> SequencesService::hasTargetSequence =
    RawContextKey::create("editorHasTargetSequence", ContextKeyBooleanValue::create(false));

SequencesService::SequencesService(sptr<IContextKeyService> context_key_service) :
  context_key_service_(context_key_service) {
  hasTargetSequence_ = hasTargetSequence->bindTo(context_key_service_);
}

void SequencesService::setTargetSequence(sptr<SequenceStorageItem> sequence) {
  target_sequence_ = sequence;
  hasTargetSequence_->set(ContextKeyBooleanValue::create(sequence ? true : false));
  onDidChangeTargetSequence(sequence);
}

sptr<SequenceStorageItem> SequencesService::target_sequence() {
  return target_sequence_;
}

}
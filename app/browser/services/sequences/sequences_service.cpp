#include "browser/services/sequences/sequences_service.h"

namespace nlive {

void SequencesService::setTargetSequence(sptr<SequenceStorageItem> sequence) {
  target_sequence_ = sequence;
  onDidChangeTargetSequence(sequence);
}

sptr<SequenceStorageItem> SequencesService::target_sequence() {
  return target_sequence_;
}

}
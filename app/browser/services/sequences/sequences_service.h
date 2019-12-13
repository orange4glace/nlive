#ifndef NLIVE_BROWSER_SERVICES_SEQUENCES_SERVICE_H_
#define NLIVE_BROWSER_SERVICES_SEQUENCES_SERVICE_H_

#include "base/common/memory.h"
#include "base/common/sig.h"
#include "platform/service/service.h"
#include "platform/include.h"
#include "model/include.h"

namespace nlive {

class SequencesService : public IService, public Sig {
DECLARE_SERVICE("nlive.browser.services.SequencesService");

public:
  static const sptr<RawContextKey> hasTargetSequence;

private:
  sptr<IContextKeyService> context_key_service_;
  sptr<SequenceStorageItem> target_sequence_;
  sptr<IContextKey> hasTargetSequence_;

public:
  SequencesService(sptr<IContextKeyService>);
  void setTargetSequence(sptr<SequenceStorageItem> sequence);

  sptr<SequenceStorageItem> target_sequence();

  sig2_t<void (sptr<SequenceStorageItem>)> onDidChangeTargetSequence;

};

}

#endif
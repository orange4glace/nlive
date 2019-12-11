#ifndef NLIVE_BROWSER_SERVICES_SEQUENCES_SERVICE_H_
#define NLIVE_BROWSER_SERVICES_SEQUENCES_SERVICE_H_

#include "base/common/memory.h"
#include "base/common/sig.h"
#include "platform/service/service.h"
#include "model/include.h"

namespace nlive {

class SequencesService : public IService, public Sig {
public:
  inline static const std::string ID = "nlive.browser.services.SequencesService";

private:
  sptr<SequenceStorageItem> target_sequence_;

public:
  SequencesService() = default;
  void setTargetSequence(sptr<SequenceStorageItem> sequence);

  sptr<SequenceStorageItem> target_sequence();

  sig2_t<void (sptr<SequenceStorageItem>)> onDidChangeTargetSequence;

  inline std::string service_id() const override { return ID; }



};

}

#endif
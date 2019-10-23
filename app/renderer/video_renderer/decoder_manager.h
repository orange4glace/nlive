#ifndef NLIVE_DECODER_MANAGER_H_
#define NLIVE_DECODER_MANAGER_H_

#include <map>
#include <vector>
#include <QSharedPointer>

#include "model/resource/video_resource.h"
#include "decoder/video_decoder.h"

namespace nlive {

namespace video_renderer {

class DecoderManager;

class VideoDecoderRef {
  private:
    QSharedPointer<VideoDecoder> decoder_;
    QSharedPointer<VideoResource> resource_;
    int referer_;
    int freq_;

    friend class DecoderManager;

  public:
    inline VideoDecoderRef(
      QSharedPointer<VideoDecoder> decoder, 
      QSharedPointer<VideoResource> resource,
      int referer) : decoder_(decoder), resource_(resource), referer_(referer),
      freq_(0) {

    }

    inline QSharedPointer<VideoDecoder> decoder() { return decoder_; }
    inline QSharedPointer<VideoResource> resource() { return resource_; }
    inline int referer() const { return referer_; }
    inline int freq() const { return freq_; }
};

class DecoderManager {

private:
  std::vector<QSharedPointer<VideoDecoderRef>> decoder_refs_;
  
public:
  DecoderManager();

  QSharedPointer<VideoDecoderRef> acquireDecoder(
      QSharedPointer<VideoResource> resource, int referer);

  void releaseDecoder(QSharedPointer<VideoDecoderRef> decoder_ref);

};

}

}

#endif
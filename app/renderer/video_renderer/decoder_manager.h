#ifndef NLIVE_VIDEO_RENDERER_DECODER_MANAGER_H_
#define NLIVE_VIDEO_RENDERER_DECODER_MANAGER_H_

#include <map>
#include <vector>
#include "base/common/memory.h"

#include "model/resource/video_resource.h"
#include "decoder/video_decoder.h"

namespace nlive {

namespace video_renderer {

class DecoderManager;

class VideoDecoderRef {
  private:
    sptr<VideoDecoder> decoder_;
    sptr<VideoResource> resource_;
    int referer_;
    int freq_;

    friend class DecoderManager;

  public:
    inline VideoDecoderRef(
      sptr<VideoDecoder> decoder, 
      sptr<VideoResource> resource,
      int referer) : decoder_(decoder), resource_(resource), referer_(referer),
      freq_(0) {

    }

    inline sptr<VideoDecoder> decoder() { return decoder_; }
    inline sptr<VideoResource> resource() { return resource_; }
    inline int referer() const { return referer_; }
    inline int freq() const { return freq_; }
};

class DecoderManager {

private:
  std::vector<sptr<VideoDecoderRef>> decoder_refs_;
  
public:
  DecoderManager();

  sptr<VideoDecoderRef> acquireDecoder(
      sptr<VideoResource> resource, int referer);

  void releaseDecoder(sptr<VideoDecoderRef> decoder_ref);

};

}

}

#endif
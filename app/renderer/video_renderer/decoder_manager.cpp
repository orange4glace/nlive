#include "renderer/video_renderer/decoder_manager.h"

#include <QDebug>

namespace nlive {

namespace video_renderer {

DecoderManager::DecoderManager() {

}

sptr<VideoDecoderRef> DecoderManager::acquireDecoder(
    sptr<VideoResource> resource, int referer) {
  sptr<VideoDecoderRef> ret = nullptr;
  int min_freq = 1;
  int idx = -1;
  int i = 0;
  for (auto decoder_ref : decoder_refs_) {
    if (decoder_ref->resource() == resource) {
      if (decoder_ref->referer() == referer) {
        ret = decoder_ref;
        idx = i;
        break;
      }
      if (decoder_ref->freq() < 0) {
        if (decoder_ref->freq() < min_freq) {
          ret = decoder_ref;
          min_freq = decoder_ref->freq();
          idx = i;
        }
      }
    }
    i++;
  }
  if (ret != nullptr) {
    decoder_refs_.erase(decoder_refs_.begin() + idx);
    return ret;
  }
  auto decoder = sptr<VideoDecoder>(new VideoDecoder(resource->path()));
  ret = sptr<VideoDecoderRef>(new VideoDecoderRef(decoder, resource, referer));
  return ret;
}

void DecoderManager::releaseDecoder(
    sptr<VideoDecoderRef> decoder_ref) {
  decoder_refs_.push_back(decoder_ref);
}

}

}
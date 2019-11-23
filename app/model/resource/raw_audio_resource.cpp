#include "model/resource/raw_audio_resource.h"

#include "base/common/memory.h"
#include <QString>
#include <cstdlib>
#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "platform/task/task.h"

namespace nlive {

const std::string RawAudioResource::TYPE = "nlive.Resource.RawAudioResource";

RawAudioResource::RawAudioResource(std::string path, int64_t ch_layout, AVSampleFormat sample_format, int sample_rate, int64_t nb_frames) :
  Resource(RawAudioResource::TYPE, path, QString("")),
  ch_layout_(ch_layout), sample_format_(sample_format),
  time_base_(sample_rate, 1), sample_rate_(sample_rate), nb_frames_(nb_frames) {

  nb_channels_ = av_get_channel_layout_nb_channels(ch_layout);
  bytes_per_sample_ = av_get_bytes_per_sample(sample_format);
  planar_ = av_sample_fmt_is_planar(sample_format);
}

}
#ifndef NLIVE_VIDEO_DECODER_H_
#define NLIVE_VIDEO_DECODER_H_

#include "base/common/memory.h"
#include <stdint.h>
#include <string>
#include <mutex>
#include <vector>
extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include "decoder/video_frame.h"

namespace nlive {

class VideoDecoder {
  
private:
  std::string path_;
  std::mutex mutex_;
  bool initialized_;
  bool opened_;
  int opener_count_;

  int width_;
  int height_;

  AVFormatContext* fmt_ctx_;
  AVCodec* dec_;
  AVCodecContext* dec_ctx_;
  AVDictionary* opts_;
  AVStream* stream_;
  AVFrame* frame_;
  AVPacket* pkt_;
  int stream_index_;
  AVPixelFormat pix_fmt_;

  int64_t last_pts_;

  int doSeek(int64_t pts);
  sptr<VideoFrame> doDecode(int64_t timestamp, bool iframe);

  void initialize();

public:
  VideoDecoder(std::string path);
  ~VideoDecoder();

  void open();
  void close();

  sptr<VideoFrame> decode(int64_t timestamp, bool iframe = false);

};

}

#endif
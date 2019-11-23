#ifndef NLIVE_VIDEO_FRAME_H_
#define NLIVE_VIDEO_FRAME_H_

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace nlive {

class VideoFrame {

private:
  AVFrame* frame_;
  int width_;
  int height_;
  AVPixelFormat pix_fmt_;

public:
  VideoFrame(AVFrame* frame, int width, int height, AVPixelFormat pix_fmt);
  ~VideoFrame();

  void scale(void* out_data, 
      AVPixelFormat dst_pix_fmt = AV_PIX_FMT_RGB32);

  inline AVFrame* frame() { return frame_; }
  inline int width() { return width_; }
  inline int height() { return height_; }
  inline AVPixelFormat pix_fmt() { return pix_fmt_; }

};

}

#endif
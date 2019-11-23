#include "decoder/video_frame.h"

namespace nlive {

VideoFrame::VideoFrame(AVFrame* frame, int width, int height, AVPixelFormat pix_fmt) :
  frame_(nullptr), width_(width), height_(height), pix_fmt_(pix_fmt) {
  frame_ = av_frame_alloc();
  av_frame_move_ref(frame_, frame);
}

VideoFrame::~VideoFrame() {
  av_frame_free(&frame_);
}

void VideoFrame::scale(void* out_data, AVPixelFormat dst_pix_fmt) {
  auto sws_ctx = sws_getContext(width_, height_, pix_fmt_, width_, height_, dst_pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);
  int linesize[4] = { width_ * 4, 0, 0, 0 };
  uint8_t* data[4] = { (uint8_t*)out_data, 0, 0, 0 };
  sws_scale(sws_ctx, frame_->data, frame_->linesize, 0, height_, data, linesize);
  sws_freeContext(sws_ctx);
}

}
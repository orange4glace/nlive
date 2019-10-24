#include "decoder/video_decoder.h"

#include <chrono>
#include <QDebug>

namespace nlive {

VideoDecoder::VideoDecoder(std::string path) :
  path_(path) {

  fmt_ctx_ = nullptr;
  dec_ = nullptr;
  dec_ctx_ = nullptr;
  opts_ = nullptr;
  stream_ = nullptr;
  pkt_ = nullptr;

  last_pts_ = AV_NOPTS_VALUE;

  opened_ = false;
  opener_count_ = 0;

  initialize();
}

void VideoDecoder::initialize() {
  qDebug() << "[VideoDecoder] initialize " << QString::fromStdString(path_) << "\n";
  initialized_ = false;
  int result;
  result = avformat_open_input(&fmt_ctx_, path_.c_str(), NULL, NULL);
  if (result != 0) {
    qDebug() << "avformat_open_input " << result << "\n";
    return;
  }
  result = avformat_find_stream_info(fmt_ctx_, NULL);
  if (result != 0) {
    qDebug() << "avformat_find_stream_info " << result << "\n";
    return;
  }
  stream_index_ = av_find_best_stream(fmt_ctx_, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  if (stream_index_ < 0) {
    qDebug() << "av_find_best_stream " << stream_index_ << "\n";
    return;
  }

  stream_ = fmt_ctx_->streams[stream_index_];
  dec_ = avcodec_find_decoder(stream_->codecpar->codec_id);
  if (dec_ == nullptr) {
    qDebug() << "avcodec_find_decoder " << dec_ << "\n";
    return;
  }
  dec_ctx_ = avcodec_alloc_context3(dec_);
  result = avcodec_parameters_to_context(dec_ctx_, stream_->codecpar);
  if (result < 0) {
    qDebug() << "avcodec_parameters_to_context " << result << "\n";
    return;
  }
  av_dict_set(&opts_, "refcounted_frames", "0", 0);

  result = avcodec_open2(dec_ctx_, dec_, &opts_);
  if (result < 0) {
    qDebug() << "avcodec_open2 " << result << "\n";
    return;
  }
  
  width_ = dec_ctx_->width;
  height_ = dec_ctx_->height;
  pix_fmt_ = dec_ctx_->pix_fmt;

  pkt_ = av_packet_alloc();
  frame_ = av_frame_alloc();

}

int VideoDecoder::doSeek(int64_t pts) {
  // std::lock_guard<std::mutex> lock(mutex_);
  avcodec_flush_buffers(dec_ctx_);
  int result = av_seek_frame(fmt_ctx_, stream_index_, pts, AVSEEK_FLAG_BACKWARD);
  if (result < 0) return result;
  return 0;
}

QSharedPointer<VideoFrame> VideoDecoder::doDecode(int64_t pts) {
  std::lock_guard<std::mutex> lock(mutex_);
  QSharedPointer<VideoFrame> frame = nullptr;
  if (last_pts_ == AV_NOPTS_VALUE
   || last_pts_ >= pts
   || last_pts_ + 50000 < pts) {
    doSeek(pts);
  }
  
  while (av_read_frame(fmt_ctx_, pkt_) >= 0) {
    int ret = 0;
    int decoded = pkt_->size;
    if (pkt_->stream_index == stream_index_) {
      ret = avcodec_send_packet(dec_ctx_, pkt_);
      if (ret < 0) {
        av_free_packet(pkt_);
        return nullptr;
      }

      ret = avcodec_receive_frame(dec_ctx_, frame_);
      if (ret >= 0) {
        if (pts <= frame_->pts) {
          frame = QSharedPointer<VideoFrame>(new VideoFrame(frame_, width_, height_, pix_fmt_));
          last_pts_ = pts;
          av_free_packet(pkt_);
          return frame;
        }
      }
    }
    av_free_packet(pkt_);
  }
  return nullptr;
}

void VideoDecoder::open() {
  std::lock_guard<std::mutex> lock(mutex_);
  opener_count_++;
  if (opened_) return;

  opened_ = true;
}

void VideoDecoder::close() {
  std::lock_guard<std::mutex> lock(mutex_);
  opener_count_--;
  if (opener_count_ == 0) {
    opened_ = false;
  }
}

QSharedPointer<VideoFrame> VideoDecoder::decode(int64_t timestamp) {
  return doDecode(timestamp);
}

}
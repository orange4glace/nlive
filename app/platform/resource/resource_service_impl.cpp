#include "platform/resource/resource_service_impl.h"

#include <QDebug>
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <map>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
}

#include "platform/logger/logger.h"
#include "platform/task/task_service.h"
#include "platform/task/task.h"
#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "model/resource/raw_audio_resource.h"
#include "model/common/rational.h"

namespace nlive {

namespace {

struct VideoMetadata {
  AVRational time_base;
  AVRational frame_rate;
  int width;
  int height;
  int64_t duration;
};

struct AudioMetadata {
  AVRational time_base;
  int sample_rate;
  int64_t duration;
};

VideoMetadata* findBestVideoMetadata(QString path) {
  av_register_all();
  AVFormatContext* fmt_ctx = nullptr;
  AVDictionaryEntry* tag = nullptr;
  int ret;
  ret = avformat_open_input(&fmt_ctx, path.toStdString().c_str(), NULL, NULL);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_open_input path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }
  ret = avformat_find_stream_info(fmt_ctx, nullptr);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_find_stream_info path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }

  VideoMetadata* metadata = nullptr;
  for (int i = 0; i < fmt_ctx->nb_streams; i ++) {
    auto stream = fmt_ctx->streams[i];
    spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Search video stream. stream idx = {} codec_type = {}", i, stream->codecpar->codec_type);
    if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      metadata = new VideoMetadata();
      metadata->time_base = stream->time_base;
      metadata->frame_rate = stream->avg_frame_rate;
      metadata->width = stream->codec->width;
      metadata->height = stream->codec->height;
      metadata->duration = stream->duration;
      spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Found video stream. time_base = {}/{} avg_frame_rate = {}/{} duration = {}",
        stream->time_base.num, stream->time_base.den, stream->avg_frame_rate.num, stream->avg_frame_rate.den, stream->duration);
      avformat_free_context(fmt_ctx);
      return metadata;
    }
  }

  avformat_free_context(fmt_ctx);
  return nullptr;
}

AudioMetadata* findBestAudioMetadata(QString path) {
  av_register_all();
  AVFormatContext* fmt_ctx = nullptr;
  AVDictionaryEntry* tag = nullptr;
  int ret;
  ret = avformat_open_input(&fmt_ctx, path.toStdString().c_str(), NULL, NULL);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_open_input path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }
  ret = avformat_find_stream_info(fmt_ctx, nullptr);
  if (ret != 0) {
    spdlog::get(LOGGER_DEFAULT)->critical("[ResourceLoadTask] Failed to avformat_find_stream_info path = {} result = {}", path.toStdString().c_str(), ret);
    return nullptr;
  }

  AudioMetadata* metadata = nullptr;
  for (int i = 0; i < fmt_ctx->nb_streams; i ++) {
    auto stream = fmt_ctx->streams[i];
    spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Search audio stream. stream idx = {} codec_type = {}", i, stream->codecpar->codec_type);
    if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      metadata = new AudioMetadata();
      metadata->time_base = stream->time_base;
      metadata->sample_rate = stream->codec->sample_rate;
      metadata->duration = stream->duration;
      spdlog::get(LOGGER_DEFAULT)->info("[ResourceLoadTask] Found aidop stream. duration = {} sample_rate = {}",
          metadata->duration, metadata->sample_rate);
      avformat_free_context(fmt_ctx);
      return metadata;
    }
  }

  avformat_free_context(fmt_ctx);
  return nullptr;
}

}

ResourceService::ResourceService(sptr<ITaskService> task_service) :
  task_service_(task_service) {
  
}

sptr<VideoResource> ResourceService::loadBestVideoResource(QString path) {
  VideoMetadata* metadata = findBestVideoMetadata(path);
  if (metadata) {
    sptr<VideoResource> resource = sptr<VideoResource>(
      new VideoResource(path.toStdString(),
          Rational::fromAVRational(metadata->time_base),
          Rational::fromAVRational(metadata->frame_rate),
          metadata->duration,
          metadata->width, metadata->height));
    delete metadata;
    return resource;
  }
  return nullptr;
}

sptr<AudioResource> ResourceService::loadBestAudioResource(QString path) {
  AudioMetadata* metadata = findBestAudioMetadata(path);
  if (metadata) {
    sptr<AudioResource> resource = sptr<AudioResource>(
      new AudioResource(path.toStdString(),
          Rational::fromAVRational(metadata->time_base),
          metadata->sample_rate,
          metadata->duration));
    // audio_resource_raw_converting_service_.process(resource);
    delete metadata;
    return resource;
  }
  return nullptr;
}

}
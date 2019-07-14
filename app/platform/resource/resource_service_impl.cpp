#include "platform/resource/resource_service_impl.h"

#include <QDebug>
#include <QThread>

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

#include "platform/task/task_service.h"
#include "platform/task/task.h"
#include "model/resource/video_resource.h"
#include "model/common/rational.h"

namespace nlive {

namespace {

struct VideoMetadata {
  AVRational time_base;
  AVRational frame_rate;
  int64_t duration;
};

VideoMetadata* findBestVideoMetadata(QString path) {
  av_register_all();
  AVFormatContext* fmt_ctx = nullptr;
  AVDictionaryEntry* tag = nullptr;
  int ret;
  ret = avformat_open_input(&fmt_ctx, path.toStdString().c_str(), NULL, NULL);
  if (ret != 0) {
    qDebug() << "Err1 " << ret << " " << path << "\n";
    return nullptr;
  }
  ret = avformat_find_stream_info(fmt_ctx, nullptr);
  if (ret != 0) {
    qDebug() << "Err2 " << ret << " " << path << "\n";
    return nullptr;
  }

  VideoMetadata* metadata = nullptr;
  for (int i = 0; i < fmt_ctx->nb_streams; i ++) {
    auto stream = fmt_ctx->streams[i];
    qDebug() << "Search video stream " << i << " " << stream->codecpar->codec_type << "\n";
    if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      metadata = new VideoMetadata();
      metadata->time_base = stream->time_base;
      metadata->frame_rate = stream->avg_frame_rate;
      metadata->duration = stream->duration;
      qDebug() << "Found video stream " << stream->time_base.num << "/" << stream->time_base.den <<
               " " << stream->avg_frame_rate.num << "/" << stream->avg_frame_rate.den << " " << stream->duration << "\n";
      avformat_free_context(fmt_ctx);
      return metadata;
    }
  }

  avformat_free_context(fmt_ctx);
  return nullptr;
}

class ResourceLoadTask : public Task {
private:
  QString path_;
  QSharedPointer<Resource> result_;
public:
  ResourceLoadTask(QString path) : path_(path), result_(nullptr) {
  }
  QSharedPointer<Resource> result() {
    return result_;
  }
protected:
  void run() {
    // Check if video
    VideoMetadata* video_metadata = findBestVideoMetadata(path_);
    if (video_metadata) {
      QSharedPointer<VideoResource> video_resource = QSharedPointer<VideoResource>(
        new VideoResource(path_, Rational::fromAVRational(video_metadata->time_base), Rational::fromAVRational(video_metadata->frame_rate), video_metadata->duration));
      result_ = video_resource;
      delete video_metadata;
      return;
    }
  }
};

}

ResourceService::ResourceService(ITaskService* task_service) :
  task_service_(task_service) {

}

void ResourceService::loadResource(QString path, std::function<void(QSharedPointer<Resource>)>&& callback) {
  ResourceLoadTask* task = new ResourceLoadTask(path);
  task_service_->queueTask(task, [callback](Task* task) {
    ResourceLoadTask* rtask = static_cast<ResourceLoadTask*>(task);
    callback(rtask->result());
  });
}

}
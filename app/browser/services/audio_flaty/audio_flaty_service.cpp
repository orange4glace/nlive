#include "browser/services/audio_flaty/audio_flaty_service.h"

#include <assert.h>
#include <QFile>
#include <QDataStream>
#include "platform/logger/logger.h"
#include "model/storage/audio_storage_item.h"
#include "model/storage/video_storage_item.h"
#include "platform/task/task.h"

namespace nlive {

class AudioResourceRawConvertingTask : public Task {

private:
  sptr<AudioResource> resource_;

protected:
  void run() override {
    const std::string& resource_path = resource_->path();
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] Start task. path = {}", resource_path.c_str());

    AVCodec* dec = nullptr;
    AVCodecContext* dec_ctx = nullptr;
    AVStream* stream;
    int stream_index;
    AVDictionary* opts = nullptr;
    AVPacket* pkt;
    AVFrame* frame;

    std::string raw_path = resource_path + ".raw";
    QFile outfile(QString::fromStdString(raw_path));
    outfile.open(QIODevice::WriteOnly);
    QDataStream outstream(&outfile);
    qDebug() << outstream.status();
    
    AVFormatContext* fmt_ctx = nullptr;
    int ret;
    ret = avformat_open_input(&fmt_ctx, resource_path.c_str(), NULL, NULL);
    if (ret != 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avformat_open_input path = {} result = {}", resource_path, ret);
      return;
    }
    ret = avformat_find_stream_info(fmt_ctx, nullptr);
    if (ret != 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avformat_find_stream_info path = {} result = {}", resource_path, ret);
      avformat_close_input(&fmt_ctx);
      return;
    }
    stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (stream_index < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to av_find_best_stream path = {} result = {}", resource_path, stream_index);
      avformat_close_input(&fmt_ctx);
      return;
    }
    stream = fmt_ctx->streams[stream_index];
    dec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (dec == nullptr) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_find_decoder path = {}", resource_path);
      avformat_close_input(&fmt_ctx);
      return;
    }
    dec_ctx = avcodec_alloc_context3(dec);
    dec_ctx->thread_count = 0;
    ret = avcodec_parameters_to_context(dec_ctx, stream->codecpar);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_parameters_to_context path = {} result = {}", resource_path, ret);
      avcodec_free_context(&dec_ctx);
      avformat_close_input(&fmt_ctx);
      return;
    }
    av_dict_set(&opts, "refcounted_frames", "0", 0);
    ret = avcodec_open2(dec_ctx, dec, &opts);
    if (ret < 0) {
      spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_open2 path = {} result = {}", resource_path, ret);
      avcodec_free_context(&dec_ctx);
      avformat_close_input(&fmt_ctx);
      return;
    }
    int sample_rate = dec_ctx->sample_rate;
    int64_t ch_layout = dec_ctx->channel_layout;
    AVSampleFormat sample_format = dec_ctx->sample_fmt;
    int nb_channels = av_get_channel_layout_nb_channels(ch_layout);
    int bytes_per_sample = av_get_bytes_per_sample(dec_ctx->sample_fmt);
    bool planar = av_sample_fmt_is_planar(dec_ctx->sample_fmt);
    int nb_frames = 0;
    pkt = av_packet_alloc();
    frame = av_frame_alloc();
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] Audio sample format = {} sample rate = {} path = {}", dec_ctx->sample_fmt, dec_ctx->sample_rate, resource_path);
    std::vector<uint8_t> buffer;
    qreal last_progress = 0;
    AVRational tb; tb.den = sample_rate; tb.num = 1;
    int64_t estimated_total_frames = av_rescale_q(stream->duration, stream->time_base,tb);
    while (av_read_frame(fmt_ctx, pkt) >= 0) {
      if (pkt->stream_index == stream_index) {
        ret = avcodec_send_packet(dec_ctx, pkt);
        if (ret < 0) {
          spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_send_packet path = {} result = {}", resource_path, ret);
          avcodec_free_context(&dec_ctx);
          avformat_close_input(&fmt_ctx);
          av_packet_free(&pkt);
          av_frame_free(&frame);
          return;
        }
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret >= 0) {
          if (planar) {
            buffer.resize(frame->nb_samples * nb_channels * bytes_per_sample);
            for (int i = 0; i < frame->nb_samples; i ++)
              for (int j = 0; j < nb_channels; j ++)
                for (int k = 0; k < bytes_per_sample; k ++) {
                  buffer[i * (nb_channels * bytes_per_sample) + j * bytes_per_sample + k] = frame->data[j][i * bytes_per_sample + k];
                }
            outstream.writeRawData((char*)buffer.data(), frame->nb_samples * bytes_per_sample * nb_channels);
          }
          else {
            outstream.writeRawData((char*)frame->data[0], frame->nb_samples * bytes_per_sample * nb_channels);
          }
          nb_frames += frame->nb_samples;
          qreal progress = nb_frames / (qreal)estimated_total_frames;
          if (last_progress + 0.01 <= progress) {
            last_progress = progress;
            setProgress(progress);
          }
          av_packet_unref(pkt);
        }
        else if (ret == AVERROR(EAGAIN)) {
          av_packet_unref(pkt);
          continue;
        }
        else {
          spdlog::get(LOGGER_DEFAULT)->critical("[AudioResourceRawConvertingTask] Failed to avcodec_receive_frame path = {} result = {}", resource_path, ret);
          avcodec_free_context(&dec_ctx);
          avformat_close_input(&fmt_ctx);
          av_packet_free(&pkt);
          av_frame_free(&frame);
          return;
        }
      }
      else {
        av_packet_unref(pkt);
      }
    }
    avcodec_free_context(&dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_packet_free(&pkt);
    av_frame_free(&frame);

    AVSampleFormat packed_sample_format = av_get_packed_sample_fmt(sample_format);
    result = sptr<RawAudioResource>(
      new RawAudioResource(raw_path, ch_layout, packed_sample_format, sample_rate, nb_frames));
    spdlog::get(LOGGER_DEFAULT)->info("[AudioResourceRawConvertingTask] "
      "End task. path = {}, ch_layout = {}, nb_channels = {}, sample_format = {}, "
      "sample_rate = {}, frames = {}", resource_path, ch_layout, nb_channels, sample_format, sample_rate, nb_frames);
  }

public:
  AudioResourceRawConvertingTask(sptr<AudioResource> resource) :
    resource_(resource) {

  }

  sptr<RawAudioResource> result;

};








Worker::Worker(sptr<ITaskService> task_service) :
  task_service_(task_service) {

}

void Worker::addWork(sptr<AudioResource> resource) {
  if (!resource) return;
  auto& path = resource->path();
  if (processed_map_.count(path)) {
    auto raw = processed_map_[path];
    resource->setRaw(raw);
    return;
  }
  if (processing_map_.count(path)) {
    processing_map_[path].push_back(resource);
    return;
  }
  processing_map_[path].push_back(resource);
  auto task = QSharedPointer<AudioResourceRawConvertingTask>(
    new AudioResourceRawConvertingTask(resource));
  task_service_->queueTask(task, [this, path](QSharedPointer<Task> task) {
    auto arrc_task = qSharedPointerCast<AudioResourceRawConvertingTask>(task);
    sptr<RawAudioResource> result = arrc_task->result;
    auto& vec = processing_map_[path];
    for (auto& v : vec) v->setRaw(result);
    processing_map_.erase(path);
    processed_map_[path] = result;
  });
}

void Worker::removeWork(sptr<AudioResource> resource) {
  if (!resource) return;
  // TODO : implement
}

DirectoryObserver::DirectoryObserver(sptr<StorageDirectory> directory, sptr<Worker> worker) :
  directory_(directory), worker_(worker) {
  spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] DirectoryObserver created. path = \"{}\"", directory->getAbsolutePath());
  directory->onDidAddItem.connect(SIG2_TRACK(sig2_t<void (sptr<StorageItem>, int index)>::slot_type(
    [this](sptr<StorageItem> item, int index) {
      if (item->type() == StorageDirectory::TYPE) {
        sptr<StorageDirectory> child_directory = std::static_pointer_cast<StorageDirectory>(item);
        auto child_observer = std::make_shared<DirectoryObserver>(child_directory, worker_);
        child_observers_[child_directory] = child_observer;
      }
      if (item->type() == AudioStorageItem::TYPE) {
        spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] AudioStorageItem added.");
        auto audio_storage_item = std::static_pointer_cast<AudioStorageItem>(item);
        worker_->addWork(audio_storage_item->audio_resource());
      }
      if (item->type() == VideoStorageItem::TYPE) {
        spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] VideoStorageItem added.");
        auto video_storage_item = std::static_pointer_cast<VideoStorageItem>(item);
        worker_->addWork(video_storage_item->audio_resource());
      }
    })));
  directory->onWillRemoveItem.connect(SIG2_TRACK(sig2_t<void (sptr<StorageItem>, int index)>::slot_type(
    [this](sptr<StorageItem> item, int index) {
      if (item->type() == StorageDirectory::TYPE) {
        sptr<StorageDirectory> child_directory = std::static_pointer_cast<StorageDirectory>(item);
        assert(child_observers_.count(child_directory));
        child_observers_.erase(child_directory);
      }
      if (item->type() == AudioStorageItem::TYPE) {
        auto audio_storage_item = std::static_pointer_cast<AudioStorageItem>(item);
        spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] AudioStorageItem removed.");
        worker_->removeWork(audio_storage_item->audio_resource());
      }
      if (item->type() == VideoStorageItem::TYPE) {
        spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] VideoStorageItem removed.");
        auto video_storage_item = std::static_pointer_cast<VideoStorageItem>(item);
        worker_->removeWork(video_storage_item->audio_resource());
      }
    })));
}

ProjectObserver::ProjectObserver(sptr<Project> project, sptr<Worker> worker) :
  project_(project), worker_(worker) {
  spdlog::get(LOGGER_DEFAULT)->info("[AudioFlatyService] ProjectObserver created.");
  root_observer_ = std::make_shared<DirectoryObserver>(project->root_storage_directory(), worker);
}

const std::string AudioFlatyService::ID = "nlive.browser.service.AudioFlatyService";

AudioFlatyService::AudioFlatyService(
    sptr<IProjectsService> projects_service,
    sptr<ITaskService> task_service) {
  worker_ = std::make_shared<Worker>(task_service);
  projects_service->onDidAddProject.connect(SIG2_TRACK(sig2_t<void (sptr<Project>)>::slot_type(
    [this](sptr<Project> project) {
      assert(project_observers_.count(project) == 0);
      auto observer = std::make_shared<ProjectObserver>(project, worker_);
      project_observers_[project] = observer;
    })));
  projects_service->onWillRemoveProject.connect(SIG2_TRACK(sig2_t<void (sptr<Project>)>::slot_type(
    [this](sptr<Project> project) {
      assert(project_observers_.count(project) == 1);
      project_observers_.erase(project);
    })));
}

}
#include "renderer/audio_renderer/render_io.h"

#include <QAudioFormat>
#include <QDebug>
#include <random>
#include <cstdlib>
#include <ctime>
#include <stdint.h>
#include <fstream>
#include <cstring>

namespace nlive {

namespace audio_renderer {

namespace {
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> dis(0, 7);
int k = 0;

}


RenderDevice::RenderDevice(QObject* parent, sptr<RenderState> render_state) :
  QIODevice(parent), render_state_(render_state) {
  byte_index_ = 0;
  buffer_size_ = render_state_->buffer()->bytes_per_slot();
  srand((unsigned int)time(NULL));
}

qint64 RenderDevice::readData(char* data, qint64 max_size) {
  if (max_size == 0) return 0;

  char* cur_data_ptr = data;

  int bytes_per_mixed_sample = render_state_->bytes_per_sample() * render_state_->nb_channels();
  int bytes_per_slot = render_state_->buffer()->bytes_per_slot();
  int max_mixed_samples = max_size / bytes_per_mixed_sample;

  int start_byte = byte_index_;
  int end_byte = start_byte + max_mixed_samples * bytes_per_mixed_sample;
  int start_consumer_slot_index = start_byte / bytes_per_slot;
  int end_consumer_slot_index = end_byte / bytes_per_slot;
  byte_index_ = end_byte;

  // TODO :
  // There's no need to check(acquiring render state lock) producer index.
  // Instead, we can store the index of the slot content into the slot itself
  // So only checking content index of the slot can assure whether slot content is valid or not
  auto& state_mutex = render_state_->state_mutex();
  state_mutex.lock();
  int producer_index = render_state_->producer_index();
  state_mutex.unlock();
  auto buffer = render_state_->buffer();

  int cur_byte = start_byte;
  for (int i = start_consumer_slot_index; i <= end_consumer_slot_index; i ++) {
    if (i >= producer_index) break;
    int this_slot_end_byte = (i + 1) * bytes_per_slot;
    int this_going_bytes = std::min(this_slot_end_byte - cur_byte, end_byte - cur_byte);
    auto& slot_mutex = render_state_->slot_mutex_at(i);
    bool locked = slot_mutex.try_lock();
    if (locked) {
      auto slot = (uint8_t*)buffer->getSlot(i);
      int slot_byte_offset = cur_byte % bytes_per_slot;
      buffer->copyTo(i, (uint8_t*)cur_data_ptr, slot_byte_offset, this_going_bytes);
      slot_mutex.unlock();
    }
    cur_byte += this_going_bytes;
    cur_data_ptr += this_going_bytes;
  }
  state_mutex.lock();
  render_state_->setConsumerIndex(end_consumer_slot_index);
  bool producer_wait = render_state_->producer_wait_flag();
  // TODO : Improve this code (replace producer_wait with something)
  if (producer_wait) render_state_->state_cv().notify_one();
  state_mutex.unlock();
  return end_byte - start_byte;
}

qint64 RenderDevice::writeData(const char* data, qint64 max_size) {
  // THIS MUST NOT BE CALLED
  Q_ASSERT(false);
  return max_size;
}

RenderIO::RenderIO(QObject* parent, sptr<RenderState> render_state) :
  QThread(parent), render_state_(render_state) {

}

void RenderIO::run() {
  QAudioFormat format;
  format.setSampleRate(render_state_->sample_rate());
  format.setChannelCount(render_state_->nb_channels());
  format.setSampleSize(render_state_->bytes_per_sample() * 8);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SampleType::Float);
  qDebug() << "Instantiate QAudioOutput.." << format.sampleRate() << format.channelCount() << format.bytesPerFrame();
  output_ = new QAudioOutput(format);
  qDebug() << "Instantiate QAudioOutput OK";
  int buffer_size = render_state_->buffer()->bytes_per_slot();
  output_->setBufferSize(buffer_size);
  qDebug() << "Instantiate RenderDevice..";
  device_ = new RenderDevice(nullptr, render_state_);
  device_->open(QIODevice::ReadOnly);
  qDebug() << "Instantiate RenderDevice OK";
  output_->start(device_);
  qDebug() << "RenderIO State = " << output_->state() << output_->bufferSize();
  exec();
}

}

}
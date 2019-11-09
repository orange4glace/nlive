#include "renderer/audio_renderer/render_io.h"

#include <QAudioFormat>
#include <QDebug>
#include <random>
#include <cstdlib>
#include <ctime>
#include <stdint.h>

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
  buffer_size_ = render_state_->bytes_per_slot();
  srand((unsigned int)time(NULL));
}

qint64 RenderDevice::readData(char* data, qint64 max_size) {
  if (max_size < buffer_size_) return 0;
  // TODO :
  // It might be safe to read ConsumerIndex without acquiring state lock
  // since writing operation is executed in nowhere but this method...
  

  auto& state_mutex = render_state_->state_mutex();
  state_mutex.lock();
  int producer_index = render_state_->producer_index();
  int consumer_index = render_state_->consumer_index();
  state_mutex.unlock();
  if (consumer_index >= producer_index) return buffer_size_;
  auto buffer = render_state_->buffer();
  auto& slot_mutex = render_state_->slot_mutex_at(consumer_index);
  bool locked = slot_mutex.try_lock();
  if (locked) {
    auto slot = (uint8_t*)buffer->getSlot(consumer_index);
    buffer->copyTo(consumer_index, (void*)data);
    slot_mutex.unlock();
  }
  state_mutex.lock();
  producer_index = render_state_->producer_index();
  consumer_index = render_state_->consumer_index();
  render_state_->setConsumerIndex(consumer_index + 1);
  bool producer_wait = render_state_->producer_wait_flag();
  // TODO : Improve this code (replace producer_wait with something)
  if (producer_wait) render_state_->state_cv().notify_one();
  state_mutex.unlock();
  return buffer_size_;
}

qint64 RenderDevice::writeData(const char* data, qint64 max_size) {
  qDebug() << "write data" << max_size;
  // NOP
  return max_size;
  Q_ASSERT(max_size == buffer_size_);
  return max_size;
}

RenderIO::RenderIO(QObject* parent, sptr<RenderState> render_state) :
  QThread(parent), render_state_(render_state) {

}

void RenderIO::run() {
  QAudioFormat format;
  format.setSampleRate(render_state_->sample_rate());
  format.setChannelCount(1);
  format.setSampleSize(32);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SampleType::Float);
  qDebug() << "Instantiate QAudioOutput..";
  output_ = new QAudioOutput(format);
  qDebug() << "Instantiate QAudioOutput OK";
  int buffer_size = render_state_->bytes_per_slot();
  output_->setBufferSize(buffer_size);
  qDebug() << "Instantiate RenderDevice..";
  device_ = new RenderDevice(nullptr, render_state_);
  device_->open(QIODevice::ReadOnly);
  qDebug() << "Instantiate RenderDevice OK";
  output_->start(device_);
  qDebug() << "RenderIO State = " << output_->state();
  exec();
}

}

}
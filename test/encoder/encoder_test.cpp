#include <QApplication>
#include <QCoreApplication>
#include <QMetaObject>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "platform/encoder/encoding_service_impl.h"
#include "model/project/project.h"
#include "model/sequence/sequence.h"
#include "model/sequence/video_clip.h"
#include "model/storage/video_storage_item.h"

using namespace nlive;

class W : public QOpenGLWidget, protected QOpenGLFunctions {

private:
  QSurfaceFormat& sf_;
  std::mutex& m_;
  std::condition_variable& cv_;
  bool& ready_;

protected:
  inline void initializeGL() override {
    std::unique_lock<std::mutex> lk(m_);
    sf_ = context()->surface()->format();
    ready_ = true;
    cv_.notify_all();
  }

public:
  inline W(QSurfaceFormat& sf, std::mutex& m, std::condition_variable& cv,
      bool& ready) :
      sf_(sf), m_(m), cv_(cv), ready_(ready) {
  }

};

TEST(Encoder, basic) {

  QSurfaceFormat sf;
  std::mutex m;
  std::condition_variable cv;
  bool sp_ready = false;
  sptr<SurfaceProvider> surface_provider;
  auto t = std::thread([&surface_provider, &m, &cv, &sp_ready, &sf]() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&sp_ready]() { return sp_ready; });
    av_register_all();
    sptr<Project> proj = std::make_shared<Project>();
    sptr<Sequence> seq = std::make_shared<Sequence>(nullptr, 30, 48000);
    sptr<VideoStorageItem> vsi = std::make_shared<VideoStorageItem>(
      proj->root_storage_directory(), "abc", std::make_shared<VideoResource>(
        "./sample.mp4",
        Rational(1, 30), Rational(1, 30), 300, 800, 450), nullptr);
    sptr<VideoClip> vc = std::make_shared<VideoClip>(nullptr, vsi->video_resource(), nullptr,
      Rational(1, 30), 48000, 0, 600, 0);
    seq->setSize(800, 450);
    seq->addTrack();
    seq->getTrackAt(0)->addClip(vc);
    seq->setDuration(90);
    EncodingTask task(seq, surface_provider, sf);
    task.start();
    QMetaObject::invokeMethod(QApplication::instance(), "quit", Qt::QueuedConnection);
  });

  int argc = 0;
  char** argv = {};
  QApplication a(argc, argv);
  surface_provider = std::make_shared<SurfaceProvider>();
  m.lock();
  m.unlock();
  W* w = new W(sf, m, cv, sp_ready);
  w->show();
  a.exec();

  t.join();
}
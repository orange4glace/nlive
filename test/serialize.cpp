#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "base/common/serialize.h"
#include "base/common/memory.h"
#include "model/project/project.h"
#include "model/sequence/sequence.h"
#include "model/storage/sequence_storage_item.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "model/serialization.h"

using namespace nlive;

TEST(Serialize, Clip) {
  auto out_clip = sptr<Clip>(new Clip(nullptr, Rational(1, 60), 48000, 350, 1350, 300));

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_clip;
  ofs.close();

  sptr<nlive::Clip> in_clip;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_clip;
  ifs.close();
  
  EXPECT_EQ(out_clip->id(), in_clip->id());
  EXPECT_EQ(out_clip->time_base(), in_clip->time_base());
  EXPECT_EQ(out_clip->sample_rate(), in_clip->sample_rate());
  EXPECT_EQ(out_clip->start_time(), in_clip->start_time());
  EXPECT_EQ(out_clip->end_time(), in_clip->end_time());
  EXPECT_EQ(out_clip->b_time(), in_clip->b_time());
}

TEST(Serialize, Track) {
  auto out_track = sptr<Track>(new Track(nullptr, Rational(1, 60), 48000));
  auto out_clip1 = sptr<Clip>(new Clip(nullptr, Rational(1, 60), 48000, 350, 1350, 300));
  auto out_clip2 = sptr<Clip>(new Clip(nullptr, Rational(1, 60), 48000, 100, 250, 0));
  out_track->addClip(out_clip1);
  out_track->addClip(out_clip2);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_track;
  ofs.close();

  sptr<nlive::Track> in_track;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_track;
  ifs.close();
  
  EXPECT_EQ(in_track->time_base(), out_track->time_base());
  EXPECT_EQ(in_track->getClipByID(out_clip1->id())->start_time(), out_clip1->start_time());
  EXPECT_EQ(in_track->getClipAt(350)->start_time(), out_clip1->start_time());
  EXPECT_EQ(in_track->getClipAt(400)->end_time(), out_clip1->end_time());
  EXPECT_EQ(in_track->getClipAt(275), nullptr);
  EXPECT_EQ(in_track->getNextClip(
    in_track->getClipByID(out_clip2->id())),
    in_track->getClipByID(out_clip1->id()));
  EXPECT_EQ(in_track->getPrevClip(
    in_track->getClipByID(out_clip1->id())),
    in_track->getClipByID(out_clip2->id()));
}

TEST(Serialize, Sequence) {
  auto out_sequence = sptr<Sequence>(new Sequence(nullptr, 60, 48000));
  auto out_track1 = out_sequence->addTrack();
  auto out_track2 = out_sequence->addTrack();
  auto out_clip1 = sptr<Clip>(new Clip(nullptr, Rational(1, 60), 48000, 350, 1350, 300));
  out_track1->addClip(out_clip1);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_sequence;
  ofs.close();

  sptr<nlive::Sequence> in_sequence;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_sequence;
  ifs.close();
  
  EXPECT_EQ(out_sequence->id(), in_sequence->id());
  EXPECT_EQ(out_sequence->name(), in_sequence->name());
  EXPECT_EQ(out_sequence->time_base(), in_sequence->time_base());
  EXPECT_EQ(out_sequence->sample_rate(), in_sequence->sample_rate());
  EXPECT_EQ(out_sequence->current_time(), in_sequence->current_time());
  EXPECT_EQ(out_sequence->duration(), in_sequence->duration());
  EXPECT_EQ(out_sequence->width(), in_sequence->width());
  EXPECT_EQ(out_sequence->height(), in_sequence->height());
  EXPECT_EQ(out_sequence->track_size(), in_sequence->track_size());
  EXPECT_EQ(out_sequence->getTrackAt(0)->getClipAt(350)->id(),
    out_clip1->id());
}
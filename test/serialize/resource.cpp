#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "base/common/serialize.h"
#include "model/resource/audio_resource.h"
#include "model/resource/video_resource.h"

using namespace nlive;

TEST(Serialize, AudioResource) {
  sptr<AudioResource> out_aresource = sptr<AudioResource>(
    new AudioResource("./audio_resource.mp3", Rational(1, 48000),
        48000, 48000 * 30));
  sptr<Resource> out_resource = std::static_pointer_cast<Resource>(out_aresource);
  
  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_resource;
  ofs.close();

  sptr<Resource> in_resource;
  sptr<AudioResource> in_aresource;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_resource;
  ifs.close();

  // EXPECT_EQ(in_resource->id(), out_resource->id());
  EXPECT_EQ(in_resource->path(), out_resource->path());
  EXPECT_EQ(in_resource->name(), out_resource->name());
  ASSERT_EQ(in_resource->type(), out_resource->type());
  in_aresource = std::static_pointer_cast<AudioResource>(in_resource);
  EXPECT_EQ(in_aresource->time_base(), out_aresource->time_base());
  EXPECT_EQ(in_aresource->sample_rate(), out_aresource->sample_rate());
  EXPECT_EQ(in_aresource->duration(), out_aresource->duration());
}

TEST(Serialize, VideoResource) {
  sptr<VideoResource> out_vresource = sptr<VideoResource>(
    new VideoResource("./video_resource.mp4", Rational(1, 60000),
        Rational(1, 30), 60000 * 30, 1920, 1080));
  sptr<Resource> out_resource = std::static_pointer_cast<Resource>(out_vresource);
  
  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_resource;
  ofs.close();

  sptr<Resource> in_resource;
  sptr<VideoResource> in_vresource;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_resource;
  ifs.close();

  // EXPECT_EQ(in_resource->id(), out_resource->id());
  EXPECT_EQ(in_resource->path(), out_resource->path());
  EXPECT_EQ(in_resource->name(), out_resource->name());
  ASSERT_EQ(in_resource->type(), out_resource->type());
  in_vresource = std::static_pointer_cast<VideoResource>(in_resource);
  EXPECT_EQ(in_vresource->time_base(), out_vresource->time_base());
  EXPECT_EQ(in_vresource->frame_rate(), out_vresource->frame_rate());
  EXPECT_EQ(in_vresource->duration(), out_vresource->duration());
  EXPECT_EQ(in_vresource->width(), out_vresource->width());
  EXPECT_EQ(in_vresource->height(), out_vresource->height());
}
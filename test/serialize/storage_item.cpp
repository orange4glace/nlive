#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "base/common/serialize.h"
#include "model/project/project.h"
#include "model/storage/audio_storage_item.h"
#include "model/storage/video_storage_item.h"
#include "model/storage/sequence_storage_item.h"

using namespace nlive;

TEST(Serialize, AudioStorageItem) {
  sptr<Project> project = sptr<Project>(new Project());
  auto root = project->root_storage_directory();
  sptr<AudioResource> out_aresource = sptr<AudioResource>(
    new AudioResource("./audio_resource.mp3", Rational(1, 48000),
        48000, 48000 * 30));
  sptr<Resource> out_resource = std::static_pointer_cast<Resource>(out_aresource);
  sptr<AudioStorageItem> out_asi = sptr<AudioStorageItem>(
    new AudioStorageItem(root, "audio_storage_item", out_aresource));
  sptr<StorageItem> out_si = std::static_pointer_cast<StorageItem>(out_asi);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_si;
  ofs.close();

  sptr<StorageItem> in_si;
  sptr<AudioStorageItem> in_asi;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_si;
  ifs.close();

  EXPECT_EQ(in_si->uuid(), out_si->uuid());
  EXPECT_EQ(in_si->name(), out_si->name());
  ASSERT_EQ(in_si->type(), out_si->type());
  in_asi = std::static_pointer_cast<AudioStorageItem>(in_si);
  EXPECT_EQ(in_asi->audio_resource()->path(), out_asi->audio_resource()->path());
}

TEST(Serialize, VideoStorageItem) {
  sptr<Project> project = sptr<Project>(new Project());
  auto root = project->root_storage_directory();
  sptr<VideoResource> out_vresource = sptr<VideoResource>(
    new VideoResource("./video_resource.mp4", Rational(1, 60000),
        Rational(1, 30), 60000 * 30, 1920, 1080));
  sptr<AudioResource> out_aresource = sptr<AudioResource>(
    new AudioResource("./audio_resource.mp3", Rational(1, 48000),
        48000, 48000 * 30));
  sptr<VideoStorageItem> out_vsi = sptr<VideoStorageItem>(
    new VideoStorageItem(root, "video_storage_item", out_vresource, nullptr));
  sptr<StorageItem> out_si = std::static_pointer_cast<StorageItem>(out_vsi);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_si;
  ofs.close();

  sptr<StorageItem> in_si;
  sptr<VideoStorageItem> in_vsi;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_si;
  ifs.close();

  EXPECT_EQ(in_si->uuid(), out_si->uuid());
  EXPECT_EQ(in_si->name(), out_si->name());
  ASSERT_EQ(in_si->type(), out_si->type());
  in_vsi = std::static_pointer_cast<VideoStorageItem>(in_si);
  EXPECT_EQ(in_vsi->video_resource()->path(), out_vsi->video_resource()->path());
  EXPECT_EQ(in_vsi->audio_resource(), nullptr);
}

TEST(Serialize, SequenceStorageItem) {
  sptr<Project> project = sptr<Project>(new Project());
  auto root = project->root_storage_directory();
  sptr<Sequence> out_seq = sptr<Sequence>(
    new Sequence(nullptr, 30, 48000));
  sptr<SequenceStorageItem> out_ssi = sptr<SequenceStorageItem>(
    new SequenceStorageItem(root, "sequence_storage_item", out_seq));
  sptr<StorageItem> out_si = std::static_pointer_cast<StorageItem>(out_ssi);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_si;
  ofs.close();

  sptr<StorageItem> in_si;
  sptr<SequenceStorageItem> in_ssi;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_si;
  ifs.close();

  EXPECT_EQ(in_si->uuid(), out_si->uuid());
  EXPECT_EQ(in_si->name(), out_si->name());
  ASSERT_EQ(in_si->type(), out_si->type());
  in_ssi = std::static_pointer_cast<SequenceStorageItem>(in_si);
  EXPECT_EQ(in_ssi->sequence()->id(), out_ssi->sequence()->id());
}

TEST(Serialize, StorageDirectory) {
  sptr<Project> project = sptr<Project>(new Project());
  auto root = project->root_storage_directory();
  sptr<StorageDirectory> subdir1 = std::make_shared<StorageDirectory>("subdir1", root);
  sptr<StorageDirectory> subdir2 = std::make_shared<StorageDirectory>("subdir2", root);
  sptr<StorageDirectory> subdir3 = std::make_shared<StorageDirectory>("subdir3", subdir1);
  sptr<Sequence> out_seq = sptr<Sequence>(
    new Sequence(nullptr, 30, 48000));
  sptr<SequenceStorageItem> out_ssi = sptr<SequenceStorageItem>(
    new SequenceStorageItem(root, "sequence_storage_item", out_seq));
  root->addItem(subdir1);
  root->addItem(subdir2);
  subdir1->addItem(subdir3);
  subdir2->addItem(out_ssi);
  sptr<StorageItem> out_si = sptr<StorageItem>(root);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_si;
  ofs.close();

  sptr<StorageItem> in_si;
  sptr<StorageDirectory> in_sd;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_si;
  ifs.close();

  EXPECT_EQ(in_si->uuid(), out_si->uuid());
  EXPECT_EQ(in_si->name(), out_si->name());
  ASSERT_EQ(in_si->type(), out_si->type());
  in_sd = std::static_pointer_cast<StorageDirectory>(in_si);
  EXPECT_EQ(in_sd->getItem(subdir1->uuid())->uuid(), subdir1->uuid());
  EXPECT_EQ(in_sd->getItem(subdir1->uuid() + "/" + subdir3->uuid())->uuid(), subdir3->uuid());
  EXPECT_EQ(in_sd->getItem(subdir2->uuid() + "/" + out_ssi->uuid())->uuid(), out_ssi->uuid());
  EXPECT_EQ(in_sd->getItem(subdir2->uuid() + "/" + out_ssi->uuid())->type(), out_ssi->type());
}
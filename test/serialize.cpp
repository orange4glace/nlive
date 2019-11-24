#include <gtest/gtest.h>
#include <boost/serialization/export.hpp>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "base/common/memory.h"
#include "model/project/project.h"
#include "model/sequence/sequence.h"
#include "model/storage/sequence_storage_item.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "model/serialization.h"

using namespace nlive;

TEST(Serialize, basic) {
  sptr<nlive::Project> out_project = std::make_shared<nlive::Project>();
  auto out_root = out_project->root_storage_directory();
  auto seq = std::make_shared<Sequence>(out_project->undo_stack(), 30, 48000);
  auto seq_item = std::make_shared<SequenceStorageItem>(out_root, "Sequence", seq);
  out_root->addItem(seq_item);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_project;
  ofs.close();

  sptr<nlive::Project> in_project;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> out_project;
  ifs.close();

  auto in_root = out_project->root_storage_directory();
  
}
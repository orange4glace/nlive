#include <gtest/gtest.h>
#include "model/project/project.h"
#include "model/storage/storage_item.h"
#include "model/storage/storage_directory.h"

using namespace nlive;

TEST(StorageItem, basic) {
  registerLoggers();
  sptr<Project> project = std::make_shared<Project>();
  sptr<StorageDirectory> root = std::make_shared<StorageDirectory>(project, "root", nullptr);
  sptr<StorageDirectory> subdir1 = std::make_shared<StorageDirectory>("subdir1", root);
  root->addItem(subdir1);

  EXPECT_EQ(root->name(), "root");
  EXPECT_EQ(subdir1->name(), "subdir1");
  EXPECT_EQ(subdir1->parent(), root);
}

TEST(StorageItem, directory) {
  sptr<Project> project = std::make_shared<Project>();
  sptr<StorageDirectory> root = std::make_shared<StorageDirectory>(project, "root", nullptr);
  sptr<StorageDirectory> subdir1 = std::make_shared<StorageDirectory>("subdir1", root);
  sptr<StorageDirectory> subdir2 = std::make_shared<StorageDirectory>("subdir2", root);
  sptr<StorageDirectory> subdir3 = std::make_shared<StorageDirectory>("subdir3", subdir1);
  root->addItem(subdir1);
  root->addItem(subdir2);
  subdir1->addItem(subdir3);

  EXPECT_EQ(root->getChildItem(subdir1->uuid()), subdir1);
  EXPECT_EQ(root->getItem(subdir1->uuid()), subdir1);

  EXPECT_EQ(root->getChildItem(subdir2->uuid()), subdir2);
  EXPECT_EQ(root->getItem(subdir2->uuid()), subdir2);

  EXPECT_EQ(root->getChildItem(subdir3->uuid()), nullptr);
  EXPECT_EQ(root->getItem(subdir3->uuid()), nullptr);

  EXPECT_EQ(subdir1->getItem(subdir3->uuid()), subdir3);
  EXPECT_EQ(subdir2->getItem(subdir3->uuid()), nullptr);

  EXPECT_EQ(subdir1->parent(), root);
  EXPECT_EQ(subdir3->parent(), subdir1);

  EXPECT_EQ(subdir1->getAbsolutePath(), root->uuid() + "/" + subdir1->uuid());
  EXPECT_EQ(subdir3->getAbsolutePath(), root->uuid() + "/" + subdir1->uuid() + "/" + subdir3->uuid());
  
  EXPECT_EQ(root->getItem(subdir1->uuid() + "/" + subdir3->uuid()), subdir3);
}
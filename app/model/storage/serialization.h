#ifndef NLIVE_MODEL_STORAGE_SERIALIZATION_H_
#define NLIVE_MODEL_STORAGE_SERIALIZATION_H_

#include "base/common/serialize.h"

#include "model/storage/storage_directory.h"
#include "model/storage/sequence_storage_item.h"
#include "model/storage/audio_storage_item.h"
#include "model/storage/video_resource_storage_item.h"

BOOST_CLASS_EXPORT_GUID(nlive::StorageDirectory, "nlive.model.storage.StorageDirectory")
BOOST_CLASS_EXPORT_GUID(nlive::SequenceStorageItem, "nlive.model.storage.SequenceStorageItem")
BOOST_CLASS_EXPORT_GUID(nlive::AudioStorageItem, "nlive.model.storage.AudioStorageItem")
BOOST_CLASS_EXPORT_GUID(nlive::VideoResourceStorageItem, "nlive.model.storage.VideoResourceStorageItem")

#endif
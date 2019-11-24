#ifndef NLIVE_MODEL_RESOURCE_SERIALIZATION_H_
#define NLIVE_MODEL_RESOURCE_SERIALIZATION_H_

#include "base/common/serialize.h"

#include "model/resource/audio_resource.h"
#include "model/resource/video_resource.h"

BOOST_CLASS_EXPORT_GUID(nlive::AudioResource, "nlive.model.resource.AudioResource")
BOOST_CLASS_EXPORT_GUID(nlive::VideoResource, "nlive.model.resource.VideoResource")

#endif
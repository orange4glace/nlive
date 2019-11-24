#ifndef NLIVE_MODEL_SEQUENCE_SERIALZATION_H_
#define NLIVE_MODEL_SEQUENCE_SERIALZATION_H_

#include "model/sequence/sequence.h"
#include "model/sequence/track.h"
#include "model/sequence/audio_clip.h"
#include "model/sequence/video_clip.h"

BOOST_CLASS_EXPORT_GUID(nlive::Sequence, "nlive.model.sequence.Sequence")
BOOST_CLASS_EXPORT_GUID(nlive::Track, "nlive.model.sequence.Track")
BOOST_CLASS_EXPORT_GUID(nlive::AudioClip, "nlive.model.sequence.AudioClip")
BOOST_CLASS_EXPORT_GUID(nlive::VideoClip, "nlive.model.sequence.VideoClip")

#endif
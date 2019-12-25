#ifndef NLIVE_MODEL_INCLUDE_H_
#define NLIVE_MODEL_INCLUDE_H_

namespace nlive {

// Common
class Rational;

// Project
class Project;

// Resource
class Resource;
class AudioResource;
class VideoResource;
class RawAudioResource;

// Sequence
class Sequence;
class Clip;
class Track;
class AudioClip;
class VideoClip;

// Storage
class StorageItem;
class StorageDirectory;
class ResourceStorageItem;
class SequenceStorageItem;
class AudioStorageItem;
class VideoStorageItem;

// Effect
namespace effect {

class IProperty;
class IKeyframe;
class Effect;
class TransformEffect;
class GrayScaleEffect;


}

}

#endif NLIVE_MODEL_INCLUDE_H_
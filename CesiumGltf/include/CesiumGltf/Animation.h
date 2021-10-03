// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/AnimationChannel.h"
#include "CesiumGltf/AnimationSampler.h"
#include "CesiumGltf/Library.h"
#include "CesiumGltf/NamedObject.h"
#include <vector>

namespace CesiumGltf {
/**
 * @brief A keyframe animation.
 */
struct CESIUMGLTF_API Animation final : public NamedObject {
  static inline constexpr const char* TypeName = "Animation";

  /**
   * @brief An array of animation channels. An animation channel combines an
   * animation sampler with a target property being animated. Different channels
   * of the same animation **MUST NOT** have the same targets.
   */
  std::vector<AnimationChannel> channels;

  /**
   * @brief An array of animation samplers. An animation sampler combines
   * timestamps with a sequence of output values and defines an interpolation
   * algorithm.
   */
  std::vector<AnimationSampler> samplers;
};
} // namespace CesiumGltf

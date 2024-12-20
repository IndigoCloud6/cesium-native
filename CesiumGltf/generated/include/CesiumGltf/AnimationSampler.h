// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/Library.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <string>

namespace CesiumGltf {
/**
 * @brief An animation sampler combines timestamps with a sequence of output
 * values and defines an interpolation algorithm.
 */
struct CESIUMGLTF_API AnimationSampler final
    : public CesiumUtility::ExtensibleObject {
  static constexpr const char* TypeName = "AnimationSampler";

  /**
   * @brief Known values for Interpolation algorithm.
   */
  struct Interpolation {
    inline static const std::string LINEAR = "LINEAR";

    inline static const std::string STEP = "STEP";

    inline static const std::string CUBICSPLINE = "CUBICSPLINE";
  };

  /**
   * @brief The index of an accessor containing keyframe timestamps.
   *
   * The accessor **MUST** be of scalar type with floating-point components. The
   * values represent time in seconds with `time[0] >= 0.0`, and strictly
   * increasing values, i.e., `time[n + 1] > time[n]`.
   */
  int32_t input = -1;

  /**
   * @brief Interpolation algorithm.
   *
   * Known values are defined in {@link Interpolation}.
   *
   */
  std::string interpolation = Interpolation::LINEAR;

  /**
   * @brief The index of an accessor, containing keyframe output values.
   */
  int32_t output = -1;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(AnimationSampler));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));

    return accum;
  }
};
} // namespace CesiumGltf

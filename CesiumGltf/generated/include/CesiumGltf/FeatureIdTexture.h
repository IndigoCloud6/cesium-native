// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Library.h"
#include "CesiumGltf/TextureInfo.h"

#include <cstdint>
#include <vector>

namespace CesiumGltf {
/**
 * @brief A texture containing feature IDs
 */
struct CESIUMGLTF_API FeatureIdTexture final : public TextureInfo {
  static inline constexpr const char* TypeName = "FeatureIdTexture";

  /**
   * @brief Texture channels containing feature IDs, identified by index.
   * Feature IDs may be packed into multiple channels if a single channel does
   * not have sufficient bit depth to represent all feature ID values. The
   * values are packed in little-endian order.
   */
  std::vector<int64_t> channels = {0};

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(FeatureIdTexture));
    accum += TextureInfo::getSizeBytes() - int64_t(sizeof(TextureInfo));
    accum += int64_t(sizeof(int64_t) * this->channels.capacity());
    return accum;
  }
};
} // namespace CesiumGltf

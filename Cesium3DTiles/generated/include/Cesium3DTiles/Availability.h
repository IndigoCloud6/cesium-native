// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "Cesium3DTiles/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <optional>

namespace Cesium3DTiles {
/**
 * @brief An object describing the availability of a set of elements.
 */
struct CESIUM3DTILES_API Availability final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName = "Availability";

  /**
   * @brief Known values for Integer indicating whether all of the elements are
   * available (1) or all are unavailable (0).
   */
  struct Constant {
    static constexpr int32_t UNAVAILABLE = 0;

    static constexpr int32_t AVAILABLE = 1;
  };

  /**
   * @brief Index of a buffer view that indicates whether each element is
   * available. The bitstream conforms to the boolean array encoding described
   * in the [3D Metadata specification](../../specification/Metadata). If an
   * element is available, its bit is 1, and if it is unavailable, its bit is 0.
   * The `bufferView` `byteOffset` must be aligned to a multiple of 8 bytes.
   */
  std::optional<int64_t> bufferView;

  /**
   * @brief A number indicating how many 1 bits exist in the availability
   * bitstream.
   */
  std::optional<int64_t> availableCount;

  /**
   * @brief Integer indicating whether all of the elements are available (1) or
   * all are unavailable (0).
   *
   * Known values are defined in {@link Constant}.
   *
   */
  std::optional<int32_t> constant;
};
} // namespace Cesium3DTiles

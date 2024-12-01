// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <Cesium3DTiles/Library.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <string>

namespace Cesium3DTiles {
/**
 * @brief 3D Tiles extension for S2 bounding volumes.
 */
struct CESIUM3DTILES_API Extension3dTilesBoundingVolumeS2 final
    : public CesiumUtility::ExtensibleObject {
  static constexpr const char* TypeName = "Extension3dTilesBoundingVolumeS2";
  static constexpr const char* ExtensionName = "3DTILES_bounding_volume_S2";

  /**
   * @brief A hexadecimal representation of the S2CellId. Tokens shall be
   * lower-case, shall not contain whitespace and shall have trailing zeros
   * stripped.
   */
  std::string token;

  /**
   * @brief The minimum height of the tile, specified in meters above (or below)
   * the WGS84 ellipsoid.
   */
  double minimumHeight = double();

  /**
   * @brief The maximum height of the tile, specified in meters above (or below)
   * the WGS84 ellipsoid.
   */
  double maximumHeight = double();

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(Extension3dTilesBoundingVolumeS2));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));
    accum += int64_t(this->token.capacity() * sizeof(char));
    return accum;
  }
};
} // namespace Cesium3DTiles

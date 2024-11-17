// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ExtensionModelMaxarMeshVariantsValue.h"
#include "CesiumGltf/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <vector>

namespace CesiumGltf {
/**
 * @brief glTF extension that defines mesh variants for nodes
 */
struct CESIUMGLTF_API ExtensionModelMaxarMeshVariants final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName =
      "ExtensionModelMaxarMeshVariants";
  static inline constexpr const char* ExtensionName = "MAXAR_mesh_variants";

  /**
   * @brief The index of the variant to load by default. The meshes that are
   * mapped to the default variant must represent the set of meshes initially
   * selected by the nodes for rendering, as per vanilla glTF behavior.
   */
  int32_t defaultProperty = -1;

  /**
   * @brief variants
   */
  std::vector<CesiumGltf::ExtensionModelMaxarMeshVariantsValue> variants;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(ExtensionModelMaxarMeshVariants));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));
    accum += int64_t(
        sizeof(CesiumGltf::ExtensionModelMaxarMeshVariantsValue) *
        this->variants.capacity());
    for (const CesiumGltf::ExtensionModelMaxarMeshVariantsValue& value :
         this->variants) {
      accum +=
          value.getSizeBytes() -
          int64_t(sizeof(CesiumGltf::ExtensionModelMaxarMeshVariantsValue));
    }
    return accum;
  }
};
} // namespace CesiumGltf

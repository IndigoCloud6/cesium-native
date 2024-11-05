// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Library.h"
#include "CesiumGltf/NamedObject.h"

#include <cstdint>
#include <optional>
#include <string>

namespace CesiumGltf {
/**
 * @brief A buffer points to binary geometry, animation, or skins.
 */
struct CESIUMGLTF_API BufferSpec : public CesiumGltf::NamedObject {
  static inline constexpr const char* TypeName = "Buffer";

  /**
   * @brief The URI (or IRI) of the buffer.
   *
   * Relative paths are relative to the current glTF asset.  Instead of
   * referencing an external file, this field **MAY** contain a `data:`-URI.
   */
  std::optional<std::string> uri;

  /**
   * @brief The length of the buffer in bytes.
   */
  int64_t byteLength = int64_t();

protected:
  /**
   * @brief This class is not meant to be instantiated directly. Use {@link Buffer} instead.
   */
  BufferSpec() = default;
  friend struct Buffer;
};
} // namespace CesiumGltf

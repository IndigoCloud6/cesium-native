// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <Cesium3DTiles/Asset.h>
#include <Cesium3DTiles/GroupMetadata.h>
#include <Cesium3DTiles/Library.h>
#include <Cesium3DTiles/MetadataEntity.h>
#include <Cesium3DTiles/Properties.h>
#include <Cesium3DTiles/Schema.h>
#include <Cesium3DTiles/Statistics.h>
#include <Cesium3DTiles/Tile.h>
#include <CesiumUtility/ExtensibleObject.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cesium3DTiles {
/**
 * @brief A 3D Tiles tileset.
 */
struct CESIUM3DTILES_API Tileset final
    : public CesiumUtility::ExtensibleObject {
  static constexpr const char* TypeName = "Tileset";

  /**
   * @brief Metadata about the entire tileset.
   */
  Cesium3DTiles::Asset asset;

  /**
   * @brief A dictionary object of metadata about per-feature properties.
   */
  std::unordered_map<std::string, Cesium3DTiles::Properties> properties;

  /**
   * @brief An object defining the structure of metadata classes and enums. When
   * this is defined, then `schemaUri` shall be undefined.
   */
  std::optional<Cesium3DTiles::Schema> schema;

  /**
   * @brief The URI (or IRI) of the external schema file. When this is defined,
   * then `schema` shall be undefined.
   */
  std::optional<std::string> schemaUri;

  /**
   * @brief An object containing statistics about metadata entities.
   */
  std::optional<Cesium3DTiles::Statistics> statistics;

  /**
   * @brief An array of groups that tile content may belong to. Each element of
   * this array is a metadata entity that describes the group. The tile content
   * `group` property is an index into this array.
   */
  std::vector<Cesium3DTiles::GroupMetadata> groups;

  /**
   * @brief A metadata entity that is associated with this tileset.
   */
  std::optional<Cesium3DTiles::MetadataEntity> metadata;

  /**
   * @brief The error, in meters, introduced if this tileset is not rendered. At
   * runtime, the geometric error is used to compute screen space error (SSE),
   * i.e., the error measured in pixels.
   */
  double geometricError = double();

  /**
   * @brief The root tile.
   */
  Cesium3DTiles::Tile root;

  /**
   * @brief Names of 3D Tiles extensions used somewhere in this tileset.
   */
  std::vector<std::string> extensionsUsed;

  /**
   * @brief Names of 3D Tiles extensions required to properly load this tileset.
   * Each element of this array shall also be contained in `extensionsUsed`.
   */
  std::vector<std::string> extensionsRequired;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += int64_t(sizeof(Tileset));
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             int64_t(sizeof(CesiumUtility::ExtensibleObject));
    accum += this->asset.getSizeBytes() - int64_t(sizeof(Cesium3DTiles::Asset));
    accum += int64_t(
        this->properties.bucket_count() *
        (sizeof(std::string) + sizeof(Cesium3DTiles::Properties)));
    for (const auto& [k, v] : this->properties) {
      accum += int64_t(k.capacity() * sizeof(char) - sizeof(std::string));
      accum += v.getSizeBytes() - int64_t(sizeof(Cesium3DTiles::Properties));
    }
    if (this->schema) {
      accum +=
          this->schema->getSizeBytes() - int64_t(sizeof(Cesium3DTiles::Schema));
    }
    if (this->schemaUri) {
      accum += int64_t(this->schemaUri->capacity() * sizeof(char));
    }
    if (this->statistics) {
      accum += this->statistics->getSizeBytes() -
               int64_t(sizeof(Cesium3DTiles::Statistics));
    }
    accum +=
        int64_t(sizeof(Cesium3DTiles::GroupMetadata) * this->groups.capacity());
    for (const Cesium3DTiles::GroupMetadata& value : this->groups) {
      accum +=
          value.getSizeBytes() - int64_t(sizeof(Cesium3DTiles::GroupMetadata));
    }
    if (this->metadata) {
      accum += this->metadata->getSizeBytes() -
               int64_t(sizeof(Cesium3DTiles::MetadataEntity));
    }
    accum += this->root.getSizeBytes() - int64_t(sizeof(Cesium3DTiles::Tile));
    accum += int64_t(sizeof(std::string) * this->extensionsUsed.capacity());
    accum += int64_t(sizeof(std::string) * this->extensionsRequired.capacity());
    return accum;
  }
};
} // namespace Cesium3DTiles

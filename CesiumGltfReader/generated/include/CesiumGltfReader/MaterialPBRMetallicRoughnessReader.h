// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <CesiumGltf/MaterialPBRMetallicRoughness.h>
#include <CesiumGltfReader/Library.h>
#include <CesiumJsonReader/JsonReader.h>
#include <CesiumJsonReader/JsonReaderOptions.h>

#include <rapidjson/fwd.h>

#include <span>
#include <vector>

namespace CesiumGltf {
struct MaterialPBRMetallicRoughness;
} // namespace CesiumGltf

namespace CesiumGltfReader {

/**
 * @brief Reads {@link MaterialPBRMetallicRoughness} instances from JSON.
 */
class CESIUMGLTFREADER_API MaterialPBRMetallicRoughnessReader {
public:
  /**
   * @brief Constructs a new instance.
   */
  MaterialPBRMetallicRoughnessReader();

  /**
   * @brief Gets the options controlling how the JSON is read.
   */
  CesiumJsonReader::JsonReaderOptions& getOptions();

  /**
   * @brief Gets the options controlling how the JSON is read.
   */
  const CesiumJsonReader::JsonReaderOptions& getOptions() const;

  /**
   * @brief Reads an instance of MaterialPBRMetallicRoughness from a byte
   * buffer.
   *
   * @param data The buffer from which to read the instance.
   * @return The result of reading the instance.
   */
  CesiumJsonReader::ReadJsonResult<CesiumGltf::MaterialPBRMetallicRoughness>
  readFromJson(const std::span<const std::byte>& data) const;

  /**
   * @brief Reads an instance of MaterialPBRMetallicRoughness from a
   * rapidJson::Value.
   *
   * @param data The buffer from which to read the instance.
   * @return The result of reading the instance.
   */
  CesiumJsonReader::ReadJsonResult<CesiumGltf::MaterialPBRMetallicRoughness>
  readFromJson(const rapidjson::Value& value) const;

  /**
   * @brief Reads an array of instances of MaterialPBRMetallicRoughness from a
   * rapidJson::Value.
   *
   * @param data The buffer from which to read the array of instances.
   * @return The result of reading the array of instances.
   */
  CesiumJsonReader::ReadJsonResult<
      std::vector<CesiumGltf::MaterialPBRMetallicRoughness>>
  readArrayFromJson(const rapidjson::Value& value) const;

private:
  CesiumJsonReader::JsonReaderOptions _options;
};

} // namespace CesiumGltfReader

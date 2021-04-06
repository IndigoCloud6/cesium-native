#pragma once

#include "CesiumGltf/ExtensionRegistry.h"
#include "CesiumGltf/Model.h"
#include "CesiumGltf/ReadModelOptions.h"
#include "CesiumGltf/ReaderLibrary.h"
#include <cstddef>
#include <gsl/span>
#include <optional>
#include <string>
#include <vector>

namespace CesiumGltf {
/**
 * @brief The result of reading a glTF model with {@link readModel}.
 */
struct CESIUMGLTFREADER_API ModelReaderResult {
  /**
   * @brief The read model, or std::nullopt if the model could not be read.
   */
  std::optional<Model> model;

  /**
   * @brief Errors, if any, that occurred during the load process.
   */
  std::vector<std::string> errors;

  /**
   * @brief Warnings, if any, that occurred during the load process.
   */
  std::vector<std::string> warnings;
};

/**
 * @brief Reads a glTF or binary glTF (GLB) from a buffer.
 *
 * @param data The buffer from which to read the glTF.
 * @param options Options for how to read the glTF.
 * @return The result of reading the glTF.
 */
CESIUMGLTFREADER_API ModelReaderResult readModel(
    const gsl::span<const std::byte>& data,
    const ReadModelOptions& options = ReadModelOptions());

/**
 * @brief The result of reading an image with {@link readImage}.
 */
struct CESIUMGLTFREADER_API ImageReaderResult {

  /**
   * @brief The {@link ImageCesium} that was read.
   *
   * This will be `std::nullopt` if the image could not be read.
   */
  std::optional<ImageCesium> image;

  /**
   * @brief Error messages that occurred while trying to read the image.
   */
  std::vector<std::string> errors;

  /**
   * @brief Warning messages that occurred while reading the image.
   */
  std::vector<std::string> warnings;
};

/**
 * @brief Reads an image from a buffer.
 *
 * The [stb_image](https://github.com/nothings/stb) library is used to decode
 * images in `JPG`, `PNG`, `TGA`, `BMP`, `PSD`, `GIF`, `HDR`, or `PIC` format.
 *
 * @param data The buffer from which to read the image.
 * @return The result of reading the image.
 */
CESIUMGLTFREADER_API ImageReaderResult
readImage(const gsl::span<const std::byte>& data);
} // namespace CesiumGltf

// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include <Cesium3DTiles/BoundingVolume.h>
#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/DoubleJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace Cesium3DTilesReader {
class BoundingVolumeJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = Cesium3DTiles::BoundingVolume;

  explicit BoundingVolumeJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void
  reset(IJsonHandler* pParentHandler, Cesium3DTiles::BoundingVolume* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyBoundingVolume(
      const std::string& objectType,
      const std::string_view& str,
      Cesium3DTiles::BoundingVolume& o);

private:
  Cesium3DTiles::BoundingVolume* _pObject = nullptr;
  CesiumJsonReader::
      ArrayJsonHandler<double, CesiumJsonReader::DoubleJsonHandler>
          _box;
  CesiumJsonReader::
      ArrayJsonHandler<double, CesiumJsonReader::DoubleJsonHandler>
          _region;
  CesiumJsonReader::
      ArrayJsonHandler<double, CesiumJsonReader::DoubleJsonHandler>
          _sphere;
};
} // namespace Cesium3DTilesReader

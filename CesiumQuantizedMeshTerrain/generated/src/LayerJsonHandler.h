// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "AvailabilityRectangleJsonHandler.h"

#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/DoubleJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>
#include <CesiumQuantizedMeshTerrain/Layer.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace CesiumQuantizedMeshTerrain {
class LayerJsonHandler : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = CesiumQuantizedMeshTerrain::Layer;

  explicit LayerJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumQuantizedMeshTerrain::Layer* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyLayer(
      const std::string& objectType,
      const std::string_view& str,
      CesiumQuantizedMeshTerrain::Layer& o);

private:
  CesiumQuantizedMeshTerrain::Layer* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _attribution;
  CesiumJsonReader::ArrayJsonHandler<
      std::vector<CesiumQuantizedMeshTerrain::AvailabilityRectangle>,
      CesiumJsonReader::ArrayJsonHandler<
          CesiumQuantizedMeshTerrain::AvailabilityRectangle,
          AvailabilityRectangleJsonHandler>>
      _available;
  CesiumJsonReader::
      ArrayJsonHandler<double, CesiumJsonReader::DoubleJsonHandler>
          _bounds;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::
      ArrayJsonHandler<std::string, CesiumJsonReader::StringJsonHandler>
          _extensionsProperty;
  CesiumJsonReader::StringJsonHandler _format;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _maxzoom;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _minzoom;
  CesiumJsonReader::IntegerJsonHandler<int64_t> _metadataAvailability;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _parentUrl;
  CesiumJsonReader::StringJsonHandler _projection;
  CesiumJsonReader::StringJsonHandler _scheme;
  CesiumJsonReader::
      ArrayJsonHandler<std::string, CesiumJsonReader::StringJsonHandler>
          _tiles;
  CesiumJsonReader::StringJsonHandler _version;
};
} // namespace CesiumQuantizedMeshTerrain

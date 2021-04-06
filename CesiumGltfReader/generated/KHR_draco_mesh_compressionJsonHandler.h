// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "DictionaryJsonHandler.h"
#include "ExtensibleObjectJsonHandler.h"
#include "IntegerJsonHandler.h"
#include <CesiumGltf/Reader.h>

namespace CesiumGltf {
struct KHR_draco_mesh_compression;

class KHR_draco_mesh_compressionJsonHandler
    : public ExtensibleObjectJsonHandler {
public:
  KHR_draco_mesh_compressionJsonHandler(
      const ReadModelOptions& options) noexcept;
  void reset(IJsonHandler* pHandler, KHR_draco_mesh_compression* pObject);
  KHR_draco_mesh_compression* getObject();
  virtual void reportWarning(
      const std::string& warning,
      std::vector<std::string>&& context = std::vector<std::string>()) override;

  virtual IJsonHandler* Key(const char* str, size_t length, bool copy) override;

protected:
  IJsonHandler* KHR_draco_mesh_compressionKey(
      const std::string& objectType,
      const char* str,
      KHR_draco_mesh_compression& o);

private:
  KHR_draco_mesh_compression* _pObject = nullptr;
  IntegerJsonHandler<int32_t> _bufferView;
  DictionaryJsonHandler<int32_t, IntegerJsonHandler<int32_t>> _attributes;
};
} // namespace CesiumGltf

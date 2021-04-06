// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#include "MeshPrimitiveJsonHandler.h"
#include "CesiumGltf/MeshPrimitive.h"

#include <cassert>
#include <string>

using namespace CesiumGltf;

MeshPrimitiveJsonHandler::MeshPrimitiveJsonHandler(
    const ReadModelOptions& options) noexcept
    : ExtensibleObjectJsonHandler(options),
      _attributes(options),
      _indices(options),
      _material(options),
      _mode(options),
      _targets(options) {}

void MeshPrimitiveJsonHandler::reset(
    IJsonHandler* pParent,
    MeshPrimitive* pObject) {
  ExtensibleObjectJsonHandler::reset(pParent, pObject);
  this->_pObject = pObject;
}

MeshPrimitive* MeshPrimitiveJsonHandler::getObject() { return this->_pObject; }

void MeshPrimitiveJsonHandler::reportWarning(
    const std::string& warning,
    std::vector<std::string>&& context) {
  if (this->getCurrentKey()) {
    context.emplace_back(std::string(".") + this->getCurrentKey());
  }
  this->parent()->reportWarning(warning, std::move(context));
}

IJsonHandler* MeshPrimitiveJsonHandler::Key(
    const char* str,
    size_t /*length*/,
    bool /*copy*/) {
  assert(this->_pObject);
  return this->MeshPrimitiveKey(MeshPrimitive::TypeName, str, *this->_pObject);
}

IJsonHandler* MeshPrimitiveJsonHandler::MeshPrimitiveKey(
    const std::string& objectType,
    const char* str,
    MeshPrimitive& o) {
  using namespace std::string_literals;

  if ("attributes"s == str)
    return property("attributes", this->_attributes, o.attributes);
  if ("indices"s == str)
    return property("indices", this->_indices, o.indices);
  if ("material"s == str)
    return property("material", this->_material, o.material);
  if ("mode"s == str)
    return property("mode", this->_mode, o.mode);
  if ("targets"s == str)
    return property("targets", this->_targets, o.targets);

  return this->ExtensibleObjectKey(objectType, str, *this->_pObject);
}

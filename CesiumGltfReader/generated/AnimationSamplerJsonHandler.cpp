// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#include "AnimationSamplerJsonHandler.h"
#include "CesiumGltf/AnimationSampler.h"

#include <cassert>
#include <string>

using namespace CesiumGltf;

AnimationSamplerJsonHandler::AnimationSamplerJsonHandler(
    const ReadModelOptions& options) noexcept
    : ExtensibleObjectJsonHandler(options),
      _input(options),
      _interpolation(options),
      _output(options) {}

void AnimationSamplerJsonHandler::reset(
    IJsonHandler* pParent,
    AnimationSampler* pObject) {
  ExtensibleObjectJsonHandler::reset(pParent, pObject);
  this->_pObject = pObject;
}

AnimationSampler* AnimationSamplerJsonHandler::getObject() {
  return this->_pObject;
}

void AnimationSamplerJsonHandler::reportWarning(
    const std::string& warning,
    std::vector<std::string>&& context) {
  if (this->getCurrentKey()) {
    context.emplace_back(std::string(".") + this->getCurrentKey());
  }
  this->parent()->reportWarning(warning, std::move(context));
}

IJsonHandler* AnimationSamplerJsonHandler::Key(
    const char* str,
    size_t /*length*/,
    bool /*copy*/) {
  assert(this->_pObject);
  return this->AnimationSamplerKey(
      AnimationSampler::TypeName,
      str,
      *this->_pObject);
}

IJsonHandler* AnimationSamplerJsonHandler::AnimationSamplerKey(
    const std::string& objectType,
    const char* str,
    AnimationSampler& o) {
  using namespace std::string_literals;

  if ("input"s == str)
    return property("input", this->_input, o.input);
  if ("interpolation"s == str)
    return property("interpolation", this->_interpolation, o.interpolation);
  if ("output"s == str)
    return property("output", this->_output, o.output);

  return this->ExtensibleObjectKey(objectType, str, *this->_pObject);
}

void AnimationSamplerJsonHandler::InterpolationJsonHandler::reset(
    IJsonHandler* pParent,
    AnimationSampler::Interpolation* pEnum) {
  JsonHandler::reset(pParent);
  this->_pEnum = pEnum;
}

IJsonHandler* AnimationSamplerJsonHandler::InterpolationJsonHandler::String(
    const char* str,
    size_t /*length*/,
    bool /*copy*/) {
  using namespace std::string_literals;

  assert(this->_pEnum);

  if ("LINEAR"s == str)
    *this->_pEnum = AnimationSampler::Interpolation::LINEAR;
  else if ("STEP"s == str)
    *this->_pEnum = AnimationSampler::Interpolation::STEP;
  else if ("CUBICSPLINE"s == str)
    *this->_pEnum = AnimationSampler::Interpolation::CUBICSPLINE;
  else
    return nullptr;

  return this->parent();
}

#pragma once

#include "CesiumGltf/ClassProperty.h"
#include "CesiumGltf/ExtensionModelExtStructuralMetadata.h"
#include "CesiumGltf/Image.h"
#include "CesiumGltf/ImageCesium.h"
#include "CesiumGltf/Model.h"
#include "CesiumGltf/PropertyTexture.h"
#include "CesiumGltf/Sampler.h"
#include "CesiumGltf/Texture.h"

#include <cassert>
#include <cstdint>
#include <variant>

namespace CesiumGltf {
/**
 * @brief Indicates the status of a property texture property view.
 *
 * The {@link PropertyTexturePropertyView} constructor always completes
 * successfully. However it may not always reflect the actual content of the
 * corresponding property texture property. This enumeration provides the
 * reason.
 */
enum class PropertyTexturePropertyViewStatus {
  /**
   * @brief This view is valid and ready to use.
   */
  Valid,

  /**
   * @brief This view has not been initialized.
   */
  ErrorUninitialized,

  /**
   * @brief This property texture property is associated with a class property
   * with an invalid or unsupported type.
   */
  ErrorInvalidClassProperty,

  /**
   * @brief This property texture property has a texture index that does not
   * exist in the glTF.
   */
  ErrorInvalidTexture,

  /**
   * @brief This property texture property has a texture sampler index that does
   * not exist in the glTF.
   */
  ErrorInvalidTextureSampler,

  /**
   * @brief This property texture property has an image index that does not
   * exist in the glTF.
   */
  ErrorInvalidImage,

  /**
   * @brief This property texture property points to an empty image.
   */
  ErrorEmptyImage,

  /**
   * @brief The channels of this property texture property are invalid.
   * Channels must be in the range 0-3, with a minimum of one channel. Although
   * more than four channels can be defined for specialized texture
   * formats, this view only supports a maximum of four channels.
   */
  ErrorInvalidChannels
};

/**
 * @brief The supported component types that can exist in property id textures.
 */
enum class PropertyTexturePropertyComponentType {
  Uint8
  // TODO: add more types. Currently this is the only one outputted by stb,
  // so change stb call to output more of the original types.
};

/**
 * @brief The property texture property value for a pixel. This will contain
 * four channels of the specified type.
 *
 * Only the first n components will be valid, where n is the number of channels
 * in this property texture property.
 *
 * @tparam T The component type, must correspond to a valid
 * {@link PropertyTexturePropertyComponentType}.
 */
template <typename T> struct PropertyTexturePropertyValue {
  T components[4];
};

/**
 * @brief A view of the data specified by a {@link PropertyTextureProperty}.
 *
 * Provides utilities to sample the property texture property using texture
 * coordinates.
 */
class PropertyTexturePropertyView {
public:
  /**
   * @brief Construct an uninitialized, invalid view.
   */
  PropertyTexturePropertyView() noexcept;

  /**
   * @brief Construct a view of the data specified by the given property texture
   * property. Assumes the model has already been validated by the
   * {@link PropertyTextureView} that invoked this constructor.
   *
   * @param model The glTF in which to look for the data specified by the
   * property texture property.
   * @param classProperty The property description.
   * @param propertyTextureProperty The property texture property
   */
  PropertyTexturePropertyView(
      const Model& model,
      const ClassProperty& classProperty,
      const PropertyTextureProperty& propertyTextureProperty) noexcept;

  /**
   * @brief Gets the unswizzled property value for the given texture
   * coordinates.
   *
   * Will return 0s when the status is not Valid or when the templated
   * component type doesn't match the image's channel byte-size.
   *
   * @tparam T The component type to use when interpreting the channels of the
   * property's pixel value.
   * @param u The u-component of the texture coordinates. Must be within
   * [0.0, 1.0].
   * @param v The v-component of the texture coordinates. Must be within
   * [0.0, 1.0].
   * @return The property at the nearest pixel to the texture coordinates.
   */
  template <typename T>
  PropertyTexturePropertyValue<T> get(double u, double v) const noexcept {
    PropertyTexturePropertyValue<T> property;
    property.components[0] = 0;
    property.components[1] = 0;
    property.components[2] = 0;
    property.components[3] = 0;

    if (this->_status != PropertyTexturePropertyViewStatus::Valid ||
        sizeof(T) != this->_pImage->bytesPerChannel) {
      return property;
    }

    double fraction = 0, integral = 0;
    int64_t integer = 0;
    switch (this->_pSampler->wrapS) {
    case Sampler::WrapS::REPEAT:
      fraction = std::modf(u, &integral);
      // Wrap negative values.
      u = fraction < 0 ? 1.0 - fraction : fraction;
      break;
    case Sampler::WrapS::MIRRORED_REPEAT:
      fraction = std::abs(std::modf(u, &integral));
      integer = static_cast<int64_t>(std::abs(integral));
      // If the integer part is odd, the direction is reversed.
      u = integer % 2 == 1 ? 1.0 - fraction : fraction;
      break;
    case Sampler::WrapS::CLAMP_TO_EDGE:
    default:
      u = std::clamp(u, 0.0, 1.0);
      break;
    }

    switch (this->_pSampler->wrapT) {
    case Sampler::WrapT::REPEAT:
      fraction = std::modf(v, &integral);
      // Wrap negative values.
      v = fraction < 0 ? 1.0 - fraction : fraction;
      break;
    case Sampler::WrapT::MIRRORED_REPEAT:
      fraction = std::abs(std::modf(v, &integral));
      integer = static_cast<int64_t>(std::abs(integral));
      // If the integer part is odd, the direction is reversed.
      v = integer % 2 == 1 ? 1.0 - fraction : fraction;
      break;
    case Sampler::WrapT::CLAMP_TO_EDGE:
    default:
      v = std::clamp(u, 0.0, 1.0);
      break;
    }

    // Clamp here to ensure no out-of-bounds data access.
    int64_t x = std::clamp(
        std::llround(u * this->_pImage->width),
        0LL,
        static_cast<int64_t>(this->_pImage->width) - 1);
    int64_t y = std::clamp(
        std::llround(v * this->_pImage->height),
        0LL,
        static_cast<int64_t>(this->_pImage->height) - 1);

    int64_t pixelOffset = this->_pImage->bytesPerChannel *
                          this->_pImage->channels *
                          (y * this->_pImage->width + x);
    const T* pRedChannel = reinterpret_cast<const T*>(
        this->_pImage->pixelData.data() + pixelOffset);

    // TODO: account for the sampler filter.
    // TODO: it is possible for channels to represent multi-byte values for a
    // property. But we only support uint8 property types at the moment.
    for (size_t i = 0; i < this->_channels.size(); i++) {
      const size_t channel = static_cast<size_t>(this->_channels[i]);
      property.components[channel] = *(pRedChannel + channel);
    }

    return property;
  }

  /**
   * @brief Get the status of this view.
   *
   * If invalid, it will not be safe to sample from this view.
   */
  PropertyTexturePropertyViewStatus status() const noexcept {
    return this->_status;
  }

  /**
   * @brief Get the component type for this property.
   */
  PropertyTexturePropertyComponentType
  getPropertyComponentType() const noexcept {
    return this->_componentType;
  }

  /**
   * @brief Get the component count of this property. This is equivalent to how
   * many channels a pixel value for this property will use.
   */
  int64_t getComponentCount() const noexcept { return this->_componentCount; }

  /**
   * @brief Get the texture coordinate set index for this property.
   */
  int64_t getTexCoordSetIndex() const noexcept {
    return this->_texCoordSetIndex;
  }

  /**
   * @brief Whether the component type for this property should be normalized.
   */
  bool isNormalized() const noexcept { return this->_normalized; }

  /**
   * @brief Get the image containing this property's data.
   *
   * This will be nullptr if the property texture property view runs into
   * problems during construction.
   */
  const ImageCesium* getImage() const noexcept { return this->_pImage; }

  /**
   * @brief Gets the channels of this property texture property.
   */
  const std::vector<int64_t>& getChannels() const noexcept {
    return this->_channels;
  }

  /**
   * @brief Gets this property's channels as a swizzle string.
   */
  const std::string& getSwizzle() const noexcept { return this->_swizzle; }

private:
  PropertyTexturePropertyViewStatus _status;
  const ClassProperty* _pClassProperty;

  const Sampler* _pSampler;
  const ImageCesium* _pImage;
  int64_t _texCoordSetIndex;
  std::vector<int64_t> _channels;
  std::string _swizzle;
  PropertyTexturePropertyComponentType _componentType;
  int64_t _componentCount;
  bool _normalized;
};
} // namespace CesiumGltf

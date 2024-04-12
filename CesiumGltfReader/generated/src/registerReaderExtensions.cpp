// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!

#include "registerReaderExtensions.h"

#include "ExtensionBufferExtMeshoptCompressionJsonHandler.h"
#include "ExtensionBufferViewExtMeshoptCompressionJsonHandler.h"
#include "ExtensionCesiumPrimitiveOutlineJsonHandler.h"
#include "ExtensionCesiumRTCJsonHandler.h"
#include "ExtensionCesiumTileEdgesJsonHandler.h"
#include "ExtensionExtInstanceFeaturesJsonHandler.h"
#include "ExtensionExtMeshFeaturesJsonHandler.h"
#include "ExtensionExtMeshGpuInstancingJsonHandler.h"
#include "ExtensionKhrDracoMeshCompressionJsonHandler.h"
#include "ExtensionKhrMaterialsUnlitJsonHandler.h"
#include "ExtensionKhrTextureBasisuJsonHandler.h"
#include "ExtensionKhrTextureTransformJsonHandler.h"
#include "ExtensionMeshPrimitiveExtStructuralMetadataJsonHandler.h"
#include "ExtensionMeshPrimitiveKhrMaterialsVariantsJsonHandler.h"
#include "ExtensionModelExtStructuralMetadataJsonHandler.h"
#include "ExtensionModelKhrMaterialsVariantsJsonHandler.h"
#include "ExtensionModelMaxarMeshVariantsJsonHandler.h"
#include "ExtensionNodeMaxarMeshVariantsJsonHandler.h"
#include "ExtensionTextureWebpJsonHandler.h"

#include <CesiumGltf/Buffer.h>
#include <CesiumGltf/BufferView.h>
#include <CesiumGltf/FeatureIdTexture.h>
#include <CesiumGltf/Material.h>
#include <CesiumGltf/MaterialNormalTextureInfo.h>
#include <CesiumGltf/MaterialOcclusionTextureInfo.h>
#include <CesiumGltf/MeshPrimitive.h>
#include <CesiumGltf/Model.h>
#include <CesiumGltf/Node.h>
#include <CesiumGltf/PropertyTextureProperty.h>
#include <CesiumGltf/Texture.h>
#include <CesiumGltf/TextureInfo.h>
#include <CesiumJsonReader/JsonReaderOptions.h>

namespace CesiumGltfReader {

void registerReaderExtensions(CesiumJsonReader::JsonReaderOptions& options) {
  (void)options;
  options.registerExtension<CesiumGltf::Model, ExtensionCesiumRTCJsonHandler>();
  options.registerExtension<
      CesiumGltf::Model,
      ExtensionModelExtStructuralMetadataJsonHandler>();
  options.registerExtension<
      CesiumGltf::Model,
      ExtensionModelKhrMaterialsVariantsJsonHandler>();
  options.registerExtension<
      CesiumGltf::Model,
      ExtensionModelMaxarMeshVariantsJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionCesiumTileEdgesJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionExtMeshFeaturesJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionMeshPrimitiveExtStructuralMetadataJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionKhrDracoMeshCompressionJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionMeshPrimitiveKhrMaterialsVariantsJsonHandler>();
  options.registerExtension<
      CesiumGltf::MeshPrimitive,
      ExtensionCesiumPrimitiveOutlineJsonHandler>();
  options.registerExtension<
      CesiumGltf::Node,
      ExtensionExtInstanceFeaturesJsonHandler>();
  options.registerExtension<
      CesiumGltf::Node,
      ExtensionExtMeshGpuInstancingJsonHandler>();
  options.registerExtension<
      CesiumGltf::Node,
      ExtensionNodeMaxarMeshVariantsJsonHandler>();
  options.registerExtension<
      CesiumGltf::Buffer,
      ExtensionBufferExtMeshoptCompressionJsonHandler>();
  options.registerExtension<
      CesiumGltf::BufferView,
      ExtensionBufferViewExtMeshoptCompressionJsonHandler>();
  options.registerExtension<
      CesiumGltf::Material,
      ExtensionKhrMaterialsUnlitJsonHandler>();
  options.registerExtension<
      CesiumGltf::Texture,
      ExtensionKhrTextureBasisuJsonHandler>();
  options.registerExtension<
      CesiumGltf::Texture,
      ExtensionTextureWebpJsonHandler>();
  options.registerExtension<
      CesiumGltf::TextureInfo,
      ExtensionKhrTextureTransformJsonHandler>();
  options.registerExtension<
      CesiumGltf::MaterialOcclusionTextureInfo,
      ExtensionKhrTextureTransformJsonHandler>();
  options.registerExtension<
      CesiumGltf::MaterialNormalTextureInfo,
      ExtensionKhrTextureTransformJsonHandler>();
  options.registerExtension<
      CesiumGltf::PropertyTextureProperty,
      ExtensionKhrTextureTransformJsonHandler>();
  options.registerExtension<
      CesiumGltf::FeatureIdTexture,
      ExtensionKhrTextureTransformJsonHandler>();
}
} // namespace CesiumGltfReader
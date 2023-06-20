#include "CesiumGltf/PropertyTextureView.h"

#include <catch2/catch.hpp>
#include <gsl/span>

#include <bitset>
#include <climits>
#include <cstddef>
#include <cstring>
#include <vector>

using namespace CesiumGltf;

TEST_CASE("Test PropertyTextureView on model without EXT_structural_metadata "
          "extension") {
  Model model;

  // Create an erroneously isolated property texture.
  PropertyTexture propertyTexture;
  propertyTexture.classProperty = "TestClass";

  PropertyTextureProperty& propertyTextureProperty =
      propertyTexture.properties["TestClassProperty"];
  propertyTextureProperty.index = 0;
  propertyTextureProperty.texCoord = 0;
  propertyTextureProperty.channels = {0};

  PropertyTextureView view(model, propertyTexture);
  REQUIRE(
      view.status() ==
      PropertyTextureViewStatus::ErrorMissingMetadataExtension);
  REQUIRE(view.getProperties().empty());

  const ClassProperty* classProperty =
      view.getClassProperty("TestClassProperty");
  REQUIRE(!classProperty);
}

TEST_CASE("Test PropertyTextureView on model without metadata schema") {
  Model model;

  ExtensionModelExtStructuralMetadata& metadata =
      model.addExtension<ExtensionModelExtStructuralMetadata>();

  PropertyTexture& propertyTexture = metadata.propertyTextures.emplace_back();
  propertyTexture.classProperty = "TestClass";

  PropertyTextureProperty& propertyTextureProperty =
      propertyTexture.properties["TestClassProperty"];
  propertyTextureProperty.index = 0;
  propertyTextureProperty.texCoord = 0;
  propertyTextureProperty.channels = {0};

  PropertyTextureView view(model, propertyTexture);
  REQUIRE(view.status() == PropertyTextureViewStatus::ErrorMissingSchema);
  REQUIRE(view.getProperties().empty());

  const ClassProperty* classProperty =
      view.getClassProperty("TestClassProperty");
  REQUIRE(!classProperty);
}

TEST_CASE("Test property texture with nonexistent class") {
  Model model;

  ExtensionModelExtStructuralMetadata& metadata =
      model.addExtension<ExtensionModelExtStructuralMetadata>();

  Schema& schema = metadata.schema.emplace();
  Class& testClass = schema.classes["TestClass"];
  ClassProperty& testClassProperty = testClass.properties["TestClassProperty"];
  testClassProperty.type = ClassProperty::Type::SCALAR;
  testClassProperty.componentType = ClassProperty::ComponentType::UINT8;

  PropertyTexture& propertyTexture = metadata.propertyTextures.emplace_back();
  propertyTexture.classProperty = "I Don't Exist";

  PropertyTextureProperty& propertyTextureProperty =
      propertyTexture.properties["TestClassProperty"];
  propertyTextureProperty.index = 0;
  propertyTextureProperty.texCoord = 0;
  propertyTextureProperty.channels = {0};

  PropertyTextureView view(model, propertyTexture);
  REQUIRE(view.status() == PropertyTextureViewStatus::ErrorClassNotFound);
  REQUIRE(view.getProperties().empty());

  const ClassProperty* classProperty =
      view.getClassProperty("TestClassProperty");
  REQUIRE(!classProperty);
}

TEST_CASE("Test property texture with nonexistent class property") {
  Model model;

  ExtensionModelExtStructuralMetadata& metadata =
      model.addExtension<ExtensionModelExtStructuralMetadata>();

  Schema& schema = metadata.schema.emplace();
  Class& testClass = schema.classes["TestClass"];
  ClassProperty& testClassProperty = testClass.properties["TestClassProperty"];
  testClassProperty.type = ClassProperty::Type::SCALAR;
  testClassProperty.componentType = ClassProperty::ComponentType::UINT8;

  PropertyTexture& propertyTexture = metadata.propertyTextures.emplace_back();
  propertyTexture.classProperty = "TestClass";

  PropertyTextureProperty& propertyTextureProperty =
      propertyTexture.properties["I Don't Exist"];
  propertyTextureProperty.index = 0;
  propertyTextureProperty.texCoord = 0;
  propertyTextureProperty.channels = {0};

  PropertyTextureView view(model, propertyTexture);
  REQUIRE(
      view.status() == PropertyTextureViewStatus::ErrorClassPropertyNotFound);
  REQUIRE(view.getProperties().empty());

  const ClassProperty* classProperty =
      view.getClassProperty("TestClassProperty");
  REQUIRE(!classProperty);
}

TEST_CASE("Test property texture with invalid property is still valid") {
  Model model;

  ExtensionModelExtStructuralMetadata& metadata =
      model.addExtension<ExtensionModelExtStructuralMetadata>();

  Schema& schema = metadata.schema.emplace();
  Class& testClass = schema.classes["TestClass"];
  ClassProperty& testClassProperty = testClass.properties["TestClassProperty"];
  testClassProperty.type = ClassProperty::Type::SCALAR;
  testClassProperty.componentType = ClassProperty::ComponentType::UINT8;

  PropertyTexture& propertyTexture = metadata.propertyTextures.emplace_back();
  propertyTexture.classProperty = "TestClass";

  PropertyTextureProperty& propertyTextureProperty =
      propertyTexture.properties["TestClassProperty"];
  propertyTextureProperty.index = -1;

  PropertyTextureView view(model, propertyTexture);
  REQUIRE(view.status() == PropertyTextureViewStatus::Valid);

  auto properties = view.getProperties();
  REQUIRE(properties.size() == 1);

  PropertyTexturePropertyView& propertyView = properties["TestClassProperty"];
  REQUIRE(propertyView.status() != PropertyTexturePropertyViewStatus::Valid);

  const ClassProperty* classProperty =
      view.getClassProperty("TestClassProperty");
  REQUIRE(classProperty == &testClassProperty);
}

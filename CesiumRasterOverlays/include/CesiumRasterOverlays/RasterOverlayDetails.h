#pragma once

#include "Library.h"

#include <CesiumGeometry/Rectangle.h>
#include <CesiumGeospatial/BoundingRegion.h>
#include <CesiumGeospatial/Projection.h>

#include <vector>

namespace CesiumRasterOverlays {
/**
 * @brief Holds details of the {@link TileRenderContent} that are useful
 * for raster overlays.
 */
struct CESIUMRASTEROVERLAYS_API RasterOverlayDetails {
  /**
   * @brief Construct an empty RasterOverlayDetails.
   */
  RasterOverlayDetails();

  /**
   * @brief Construct a RasterOverlayDetails with given projections, the
   * rectangles generated by the projections, and the bounding region covered
   * those rectangles.
   *
   * @param rasterOverlayProjections The raster overlay projections for which
   * texture coordinates have been generated.
   * @param rasterOverlayRectangles The rectangle covered by this tile in each
   * of the {@link rasterOverlayProjections}.
   * @param boundingRegion The precise bounding region of this tile.
   */
  RasterOverlayDetails(
      std::vector<CesiumGeospatial::Projection>&& rasterOverlayProjections,
      std::vector<CesiumGeometry::Rectangle>&& rasterOverlayRectangles,
      const CesiumGeospatial::BoundingRegion& boundingRegion);

  /**
   * @brief Finds the rectangle corresponding to a given projection in
   * {@link rasterOverlayProjections}.
   *
   * @param projection The projection.
   * @return The tile's rectangle in the given projection, or nullptr if the
   * projection is not in {@link rasterOverlayProjections}.
   */
  const CesiumGeometry::Rectangle* findRectangleForOverlayProjection(
      const CesiumGeospatial::Projection& projection) const;

  /**
   * @brief Merge the other RasterOverlayDetails's projections, rectangles, and
   * bounding region together.
   *
   * @param other The other instance of RasterOverlayDetails that will be merged
   * with this.
   */
  void merge(
      const RasterOverlayDetails& other,
      const CesiumGeospatial::Ellipsoid& ellipsoid CESIUM_DEFAULT_ELLIPSOID);

  /**
   * @brief The raster overlay projections for which texture coordinates have
   * been generated.
   *
   * For the projection at index `n`, there is a set of texture coordinates
   * with the attribute name `_CESIUMOVERLAY_n` that corresponds to that
   * projection.
   */
  std::vector<CesiumGeospatial::Projection> rasterOverlayProjections;

  /**
   * @brief The rectangle covered by this tile in each of the
   * {@link rasterOverlayProjections}.
   */
  std::vector<CesiumGeometry::Rectangle> rasterOverlayRectangles;

  /**
   * @brief The precise bounding region of this tile.
   */
  CesiumGeospatial::BoundingRegion boundingRegion;
};
} // namespace CesiumRasterOverlays

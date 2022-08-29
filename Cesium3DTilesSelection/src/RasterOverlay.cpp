#include "Cesium3DTilesSelection/RasterOverlay.h"

#include "Cesium3DTilesSelection/RasterOverlayCollection.h"
#include "Cesium3DTilesSelection/RasterOverlayLoadFailureDetails.h"
#include "Cesium3DTilesSelection/RasterOverlayTileProvider.h"
#include "Cesium3DTilesSelection/spdlog-cesium.h"

using namespace CesiumAsync;
using namespace Cesium3DTilesSelection;

namespace {
class PlaceholderTileProvider : public RasterOverlayTileProvider {
public:
  PlaceholderTileProvider(
      RasterOverlay& owner,
      const CesiumAsync::AsyncSystem& asyncSystem,
      const std::shared_ptr<IAssetAccessor>& pAssetAccessor) noexcept
      : RasterOverlayTileProvider(owner, asyncSystem, pAssetAccessor) {}

  virtual CesiumAsync::Future<LoadedRasterOverlayImage>
  loadTileImage(RasterOverlayTile& /* overlayTile */) override {
    return this->getAsyncSystem()
        .createResolvedFuture<LoadedRasterOverlayImage>({});
  }
};
} // namespace

RasterOverlay::RasterOverlay(
    const std::string& name,
    const RasterOverlayOptions& options)
    : _name(name), _pPlaceholder(), _options(options), _referenceCount(0) {}

RasterOverlay::~RasterOverlay() noexcept { assert(this->_referenceCount == 0); }

void RasterOverlay::addReference() noexcept { ++this->_referenceCount; }

void RasterOverlay::releaseReference() noexcept {
  assert(this->_referenceCount > 0);
  --this->_referenceCount;
  if (this->_referenceCount == 0) {
    delete this;
  }
}

void RasterOverlay::reportError(
    const AsyncSystem& asyncSystem,
    const std::shared_ptr<spdlog::logger>& pLogger,
    RasterOverlayLoadFailureDetails&& errorDetails) {
  SPDLOG_LOGGER_ERROR(pLogger, errorDetails.message);
  if (this->getOptions().loadErrorCallback) {
    asyncSystem.runInMainThread(
        [this, errorDetails = std::move(errorDetails)]() mutable {
          this->getOptions().loadErrorCallback(std::move(errorDetails));
        });
  }
}

#pragma once

#include "CesiumGeospatial/Library.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "CesiumGeospatial/Ellipsoid.h"
#include "CesiumGeometry/Plane.h"

namespace CesiumGeospatial {

    /**
     * @brief A plane tangent to an {@link Ellipsoid} at a certain origin position.
     * 
     * If the origin is not on the surface of the ellipsoid, its surface projection 
     * will be used. 
     */
    class CESIUMGEOSPATIAL_API EllipsoidTangentPlane {
    public:

        /**
         * @brief Creates a new instance.
         * 
         * @param origin The origin, in cartesian coordinates.
         * @param ellipsoid The ellipsoid. Default value: {@link Ellipsoid::WGS84}.
         * @throws An `std::bad_optional_access` if the given origin is at the
         * center of the ellipsoid.
         */
        EllipsoidTangentPlane(
            const glm::dvec3& origin,
            const Ellipsoid& ellipsoid = Ellipsoid::WGS84
        );

        /**
         * @brief Creates a new instance.
         *
         * @param eastNorthUpToFixedFrame A transform that was computed with 
         * {@link Transforms::eastNorthUpToFixedFrame}.
         * @param ellipsoid The ellipsoid. Default value: {@link Ellipsoid::WGS84}.
         */
        EllipsoidTangentPlane(
            const glm::dmat4& eastNorthUpToFixedFrame,
            const Ellipsoid& ellipsoid = Ellipsoid::WGS84
        );

        /**
         * @brief Returns the {@link Ellipsoid}.
         */
        const Ellipsoid& getEllipsoid() const { return this->_ellipsoid; }

        /**
         * @brief Returns the origin, in cartesian coordinates.
         */
        const glm::dvec3& getOrigin() const { return this->_origin; }

        /**
         * @brief Returns the x-axis of this plane.
         */
        const glm::dvec3& getXAxis() const { return this->_xAxis; }

        /**
         * @brief Returns the y-axis of this plane.
         */
        const glm::dvec3& getYAxis() const { return this->_yAxis; }

        /**
         * @brief Returns the z-axis (i.e. the normal) of this plane.
         */
        const glm::dvec3& getZAxis() const { return this->_plane.getNormal(); }

        /**
         * @brief Returns a {@link CesiumGeometry::Plane} representation of this plane.
         */
        const CesiumGeometry::Plane& getPlane() const { return this->_plane; }

        /**
         * @brief Computes the position of the projection of the given point on this plane.
         * 
         * Projects the given point on this plane, along the normal. The result will 
         * be a 2D point, referring to the local coordinate system of the plane that 
         * is given by the x- and y-axis.
         * 
         * @param cartesian The point in cartesian coordinates.
         * @return The 2D representation of the point on the plane that is closest to
         * the given position.
         */
        glm::dvec2 projectPointToNearestOnPlane(const glm::dvec3& cartesian);

    private:
        Ellipsoid _ellipsoid;
        glm::dvec3 _origin;
        glm::dvec3 _xAxis;
        glm::dvec3 _yAxis;
        CesiumGeometry::Plane _plane;
    };

}

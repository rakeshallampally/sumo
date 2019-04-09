/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2004-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    DynamicPolygon.h
/// @author  Leonhard Luecken
/// @date    Apr 2019
/// @version $Id$
///
// A polygon, which holds a timeSpan for displaying dynamic properties
/****************************************************************************/


#ifndef UTILS_SHAPES_POLYGONDYNAMICS_H_
#define UTILS_SHAPES_POLYGONDYNAMICS_H_

#include <memory>
#include "SUMOPolygon.h"
#include "utils/common/SUMOTime.h"

class SUMOTrafficObject;
class ShapeContainer;
class SUMORTree;

class PolygonDynamics {
public:
    /**
     * @brief Constructor that takes a SUMOPolygon and adds timelines for the properties to be modified dynamically.
     * @param p Polygon to be modified
     * @param trackedObject A tracked object (nullptr indicates that no tracking is desired)
     * @param timeSpan Anchor time points
     *        For animated polygons: assumed to have a size >= 2, and start at timeSpan[0]=0,
     *        such that timeSpan[i+1] >= timeSpan[i])
     *        If no animation is desired, give timeSpan == nullptr
     * @param ...Span property timelines (assumed to be either nullptr, or of size equal to timeSpan (in case of animated poly))
     * @param looped Whether the animation should restart when the last keyframe is reached. In that case
                     the animation jumps to the first keyframe as soon as the last is reached.
                     If looped==false, the controlled polygon is removed as soon as the timeSpan elapses.
     */
    PolygonDynamics(double creationTime,
            SUMOPolygon* p,
            SUMOTrafficObject* trackedObject,
            const std::vector<double>& timeSpan,
            const std::vector<double>& alphaSpan,
            bool looped);
    virtual ~PolygonDynamics();

    const std::string& getPolygonID() const {
        return myPolygon->getID();
    }

    SUMOPolygon* getPolygon() const {
        return myPolygon;
    }

    inline const std::string& getTrackedObjectID() const {
        return myTrackedObjectID;
    }

    /// @brief Updates the polygon according to its timeSpan and follows the tracked object
    /// @param t Current sumo time step (in ms.) when this is called
    /// @returns Next desired update time.
   SUMOTime update(SUMOTime t);

   /// @brief Set the RTree
   void setRTree(SUMORTree* rtree) {
       myVis = rtree;
   }

private:

    /// @brief Sets the alpha value for the shape's color
    void setAlpha(double alpha);

    /// @brief Initialize the object's position
    void initTrackedPosition();

    /// @brief The polygon this dynamics acts upon.
    SUMOPolygon* myPolygon;

    /// @brief Current time
    double myCurrentTime;

    /// @brief The last time the animation has been updated
    double myLastUpdateTime;

    /// @brief Whether this polygon is animated, i.e., whether
    ///        timelines should be used to control properties.
    bool animated;

    /// @brief Whether animation should be looped.
    bool looped;

    /// @brief Whether this polygon tracks an object
    bool tracking;

    /// @brief An object tracked by the shape, deletion by caller
    SUMOTrafficObject* myTrackedObject;
    std::string myTrackedObjectID;

    /// @brief Initial position of the tracked object
    std::unique_ptr<Position> myTrackedObjectsInitialPositon;

    /// @brief the original shape of the polygon
    std::unique_ptr<PositionVector> myOriginalShape;

    /// @brief Time points corresponding to the anchor values of the dynamic properties
    /// @note  Assumed to have a size >= 2, and start at timeSpan[0]=0, such that timeSpan[i+1] >= timeSpan[i]
    std::unique_ptr<std::vector<double> > myTimeSpan;

    /// @brief Pointer to the next time points in timeSpan
    /// @note  These iterators are only valid if timeSpan != nullptr
    std::vector<double>::const_iterator myPrevTime;
    std::vector<double>::const_iterator myNextTime;

    /// @brief Alpha values corresponding to
    std::unique_ptr<std::vector<double> > myAlphaSpan;
    /// @brief Pointer to the next alpha points in alphaSpan
    /// @note  These iterators are only valid if alphaSpan != nullptr
    std::vector<double>::const_iterator myPrevAlpha;
    std::vector<double>::const_iterator myNextAlpha;

    /// @brief RTree will be supplied in case of GUI simulation to be updated on move
    SUMORTree* myVis;

};

#endif /* UTILS_SHAPES_POLYGONDYNAMICS_H_ */
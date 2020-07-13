/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 *  2018-2020 Pierre Cabrera <pierre.cab@gmail.com>
 */
#include "locallabspot.h"

#include "multilangmgr.h"
#include "../rtengine/procparams.h"
#include "../rtengine/rt_math.h"

using namespace rtengine::procparams;

/* ==== BaseSpot ==== */
BaseSpot::BaseSpot(EditDataProvider* const provider):
    name(M("TP_LOCALLAB_SPOTNAME")),
    shape(spotEllipse),
    isexcluded(false),
    isvisible(true),

    // Center circle widgets
    centerVisible(new Circle()),
    centerMouseOver(new Circle())
{
    // Parameter center circle
    LocallabParams::LocallabSpot defSpot;
    rtengine::Coord centerCoord;

    if (provider) { // Set newly created spot center to preview area center
        int imW = 0, imH = 0, prX = 0, prY = 0;
        provider->getImageSize(imW, imH);
        provider->getPreviewCenterPos(prX, prY);

        if (imW && imH) { // Image loaded
            centerCoord.set(prX, prY);
        }
    }

    centerVisible->center = centerCoord;
    centerVisible->radius = defSpot.circrad;
    centerVisible->datum = Geometry::IMAGE;
    centerVisible->radiusInImageSpace = true;
    centerMouseOver->center = centerCoord;
    centerMouseOver->radius = defSpot.circrad;
    centerMouseOver->datum = Geometry::IMAGE;
    centerMouseOver->radiusInImageSpace = true;

    // Add center circle to geometry lists
    visible.push_back(centerVisible);
    mouseOver.push_back(centerMouseOver);
}

BaseSpot::~BaseSpot()
{
    // Delete edit widgets
    delete centerVisible;
    delete centerMouseOver;
}

bool BaseSpot::isSpotWidget(Geometry* const visibleWidget)
{
    for (auto widget : visible) {
        if (visibleWidget == widget) {
            return true;
        }
    }

    return false;
}

BaseSpot::spotEvent BaseSpot::dragWidget(EditDataProvider* const provider, Geometry* const visibleWidget, const int modifierKey)
{
    spotEvent event = none;

    if (provider) {
        if (visibleWidget == centerVisible) {
            const bool isShiftPressed = modifierKey & Gdk::SHIFT_MASK;

            if (isShiftPressed) {
                event = centerResized;

                // Get delta and origin coord
                const rtengine::Coord deltaCoord = provider->deltaImage;
                const rtengine::Coord originCoord = provider->posImage;

                // Resize center widget
                const rtengine::Coord mouseCoord(originCoord.x + deltaCoord.x, originCoord.y + deltaCoord.y);
                const int newRadius = rtengine::norm2(centerVisible->center.x - mouseCoord.x, centerVisible->center.y - mouseCoord.y);
                centerVisible->radius = rtengine::LIM(newRadius, 2, 150);
                centerMouseOver->radius = rtengine::LIM(newRadius, 2, 150);
            } else {
                event = spotMoved;

                // Get image size
                int imW = 0, imH = 0;
                provider->getImageSize(imW, imH);

                // Get delta coord
                const rtengine::Coord deltaCoord = provider->deltaPrevImage;

                // Drag center widget
                centerVisible->center.x = rtengine::LIM(centerVisible->center.x + deltaCoord.x, 0, imW);
                centerVisible->center.y = rtengine::LIM(centerVisible->center.y + deltaCoord.y, 0, imH);
                centerMouseOver->center.x = rtengine::LIM(centerMouseOver->center.x + deltaCoord.x, 0, imW);
                centerMouseOver->center.y = rtengine::LIM(centerMouseOver->center.y + deltaCoord.y, 0, imH);
            }
        }
    }

    return event;
}

void BaseSpot::getSpotCursor(Geometry* const visibleWidget, CursorShape& cursor)
{
    if (visibleWidget == centerVisible) {
        cursor = CSMove2D;
    }
}

void BaseSpot::prelightWidget(const bool prelight)
{
    if (prelight) {
        // Center visible widget is prelighted
        centerVisible->state = Geometry::PRELIGHT;
    } else {
        // Center visible widget is unprelighted
        centerVisible->state = Geometry::NORMAL;
    }
}

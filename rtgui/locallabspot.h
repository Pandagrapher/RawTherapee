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
#pragma once

#include "editwidgets.h"
#include "editcallbacks.h"

/* ==== BaseSpot ==== */
class BaseSpot
{
public:
    // Spot shape type enumeration
    enum spotType {
        spotEllipse = 0,
        spotRectangle = 1
    };

    // Spot event type enumeration
    enum spotEvent {
        none = 0,
        spotMoved = 1,
        centerResized = 2,
        shapeChanged = 3
    };

private:
    // BaseSpot parameters
    Glib::ustring name;
    spotType shape;
    bool isexcluded;
    bool isvisible;

    // Geometry lists
    std::vector<Geometry*> visible;
    std::vector<Geometry*> mouseOver;

    // Center circle widgets
    Circle* const centerVisible;
    Circle* const centerMouseOver;

public:
    BaseSpot(EditDataProvider* const provider);
    virtual ~BaseSpot();

    // Getter/Setter functions
    Glib::ustring getSpotName()
    {
        return this->name;
    };
    void setSpotName(const Glib::ustring& name)
    {
        this->name = name;
    };
    spotType getSpotType()
    {
        return this->shape;
    };
    void setSpotType(const spotType type)
    {
        this->shape = type;
    };
    bool isSpotExcluded()
    {
        return this->isexcluded;
    };
    void setSpotExcluded(const bool excluded)
    {
        this->isexcluded = excluded;
    };
    bool isSpotVisible()
    {
        return this->isvisible;
    };
    void setSpotVisible(const bool visible)
    {
        this->isvisible = visible;
    };
    std::vector<Geometry*> getVisible()
    {
        return this->visible;
    };
    std::vector<Geometry*> getMouseOver()
    {
        return this->mouseOver;
    };

    // Spot widgets event functions
    virtual bool isSpotWidget(Geometry* const visibleWidget); // Return true if visibleWidget belongs to the spot
    virtual spotEvent dragWidget(EditDataProvider* const provider, Geometry* const visibleWidget, const int modifierKey); // Return spotEvent according to dragged visibleWidget
    virtual void getSpotCursor(Geometry* const visibleWidget, CursorShape& cursor); // Update cursor if visibleWidget belongs to the spot

    // Spot widgets appearance functions
    virtual void prelightWidget(const bool prelight = true);

private:
    // TBD
};
